#include "pch.h"
#include "codeGenerator.h"
#include "braze_compiler.h"

#define C_STACK_ALIGNMENT 16
#define STACK_PUSH_SIZE 4
#define C_ALIGN(size) (size % C_STACK_ALIGNMENT) ? size + (C_STACK_ALIGNMENT - (size % C_STACK_ALIGNMENT)) : size


codeGenerator::codeGenerator()
{
}

void codeGenerator::initialize()
{
  int error = m_asm_writer.initialize(output_file);

  m_resolver.initialize();

  if (error != 0)
  {
    cerror("failed to open file");
  }
}

void codeGenerator::setFileName(std::string filename)
{
  m_input_file = filename;
  output_file = filename + ".asm";
}

int codeGenerator::startCodeGeneration()
{
  /*

  1. generate root scope

  2. generate data section

  3. generate root

  4. finish scope

  5. generate read only data

  */

  initialize();

  m_root_scope.init(std::make_shared<scope>(), true);

  generateDataSection();

  generateRoot();


  return 0;
}

void codeGenerator::generateDataSection()
{
  m_asm_writer.asmPush("section .data");

  for (auto node : m_ast)
  {
    if (node->getNodeType() == NODE_TYPE_VARIABLE)
    {
      generateGlobalVariable(node);
    }
  }
}

void codeGenerator::generateRoot()
{
  m_asm_writer.asmPush("section .text");
  
  for (auto node : m_ast)
  {
    if (node->getNodeType() == NODE_TYPE_VARIABLE)
    {
      //already processed in generateDataSection
    }
    if (node->getNodeType() == NODE_TYPE_FUNCTION)
    {
      generateFunction(node);
    }
  }
}

void codeGenerator::generateRootNode(std::shared_ptr<node> node)
{
  if (node->getNodeType() == NODE_TYPE_FUNCTION)
  {
    m_resolver.registerFunction(node);
    std::string function_name = node->getStringValue();
    m_asm_writer.asmPush("global " + function_name);
    m_asm_writer.asmPush(function_name + ":");
    m_asm_writer.asmPush("push ebp");
    m_asm_writer.asmPush("mov ebp, esp");
    m_asm_writer.asmPush("sub esp, " + std::to_string(C_ALIGN(node->getBodyNode()->getBodySize())));
    //add function parameters
    m_resolver.createNewScope(true, false); 

    //add body generation
    generateBody(node->getBodyNode());


    m_resolver.removeScope();



    m_asm_writer.asmPush("add esp, " + std::to_string(C_ALIGN(node->getBodyNode()->getBodySize())));
    m_asm_writer.asmPush("pop ebp");
    m_asm_writer.asmPush("ret");
  }
}

void codeGenerator::generateFunction(std::shared_ptr<node> node)
{
  //deal with forward declaration
  generateRootNode(node);
}

void codeGenerator::generateBody(std::shared_ptr<node> node)
{
  m_resolver.createNewScope(false, true);
  generateScope(node);
  m_resolver.removeScope();
}

void codeGenerator::generateScope(std::shared_ptr<node> node)
{
  if (node)
  {
    if (node->getNodeType() == NODE_TYPE_BODY)
    {
      for (auto statement : node->getStatements())
      {
        generateStatement(statement);
      }
    }
  }
}

void codeGenerator::generateStatement(std::shared_ptr<node> node)
{

  if (node->getNodeType() == NODE_TYPE_VARIABLE)
  {
    generateScopedVariable(node);
    return;
  }
  if (node->getNodeType() == NODE_TYPE_EXPRESSION)
  {
    generateExpNode(node);
    return;
  }
  std::cout << "Codegen: statement not yet implemented" << std::endl;
}


void codeGenerator::generateGlobalVariable(std::shared_ptr < node > node)
{
  if (node)
  {
    if (node->getNodeType() == NODE_TYPE_VARIABLE)
    {
      std::shared_ptr < datatype > datatype = node->getDatatype();
      
      //do not deal with structs and unions here!
      if (datatype->isPrimitiveType())
      {
        generateGlobalVariablePrimitive(node);
      }
    }
  }
}

void codeGenerator::generateGlobalVariablePrimitive(std::shared_ptr<node> node)
{
  if (node->getNodeType() == NODE_TYPE_STRING)
  {
    cerror("Codegen: strings not yet implemented"); 
    return;
  }
  std::shared_ptr<resolverEntity> entity  = m_resolver.addEntity(node, false, false);

  std::string var_name = node->getStringValue();
  std::string var_value = "0";
  std::shared_ptr < datatype > datatype = node->getDatatype();
  if (node->getValueNode())
  {
    var_value = std::to_string(node->getValueNode()->getNumberValue());
  }
  m_asm_writer.asmPush(var_name + ": " + datatype->getStringForPrimitiveSize() + " " + var_value);
}

void codeGenerator::generateScopedVariable(std::shared_ptr<node> node)
{
  if (node->getNodeType() == NODE_TYPE_VARIABLE)
  {
    std::shared_ptr<resolverEntity> entity = m_resolver.addEntity(node,true);
    if (node->getValueNode())
    {
      generateExpressionable(node->getValueNode(), IS_ASSIGNMENT | IS_RIGHT_HAND_OF_ASSIGNMENT);
      m_asm_writer.asmPush("pop eax");
      std::string reg_to_use = "eax";
      std::string mov_type = node->getDatatype()->getDatatypeRegisterSize();
      node->getDatatype()->getRegToUse(reg_to_use);
      generateAssignmentInstructionForOperator(mov_type, entity->getAddress(), reg_to_use, "=");
    }
  }
}

void codeGenerator::generateExpressionable(std::shared_ptr<node> node, int flags)
{
  //flags |= IS_NOT_ROOT;

  if (node->getNodeType() == NODE_TYPE_IDENTIFIER)
  {
    generateIdentifier(node);
  }
  else if (node->getNodeType() == NODE_TYPE_NUMBER)
  {
    generateNumber(node, flags);
  }
  else if (node->getNodeType() == NODE_TYPE_STRING)
  {

  }

}

void codeGenerator::generateExpNode(std::shared_ptr<node> node)
{
  if (node->isAssignmentNode())
  {
    generateAssignmentExpression(node);
    return;
  }
  //try to resolve node!

  //might be function call

}

void codeGenerator::generateAssignmentExpression(std::shared_ptr<node> node)
{
  generateExpressionable(node->getRightNode(), IS_ASSIGNMENT | IS_RIGHT_HAND_OF_ASSIGNMENT);
}

void codeGenerator::generateNumber(std::shared_ptr<node> node, int flags)
{
  //todo add stack verificatoions
  m_asm_writer.asmPush("push dword " + std::to_string(node->getNumberValue()) );
}

void codeGenerator::generateIdentifier(std::shared_ptr<node> node)
{
  std::shared_ptr<resolverEntity> entity = m_resolver.follow(node);
}

void codeGenerator::generateAssignmentPart(std::shared_ptr<node> node, int flags)
{

  std::shared_ptr<resolverEntity> entity = m_resolver.follow(node);
  if (entity)
  {
    m_asm_writer.asmPush("pop eax");
    m_asm_writer.asmPush("mov eax " + entity->getAddress());
  }

}

void codeGenerator::generateAssignmentInstructionForOperator(std::string mov_type, std::string address, std::string reg_to_use, std::string _operator)
{
  if (STRINGS_EQUAL(_operator.c_str(), "="))
  {
    m_asm_writer.asmPush("mov " + mov_type +" ["+ address+"], "+ reg_to_use);
  }
  else if (STRINGS_EQUAL(_operator.c_str(), "+="))
  {
    m_asm_writer.asmPush("add " + mov_type + " [" + address + "], " + reg_to_use);
  }
}
