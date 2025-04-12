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
      generateRootNode(node);
    }
  }
}

void codeGenerator::generateRootNode(std::shared_ptr<node> node)
{
  if (node->getNodeType() == NODE_TYPE_FUNCTION)
  {
    std::string function_name = node->getStringValue();
    m_asm_writer.asmPush("global " + function_name);
    m_asm_writer.asmPush(function_name + ":");
    m_asm_writer.asmPush("push ebp");
    m_asm_writer.asmPush("mov ebp, esp");
    m_asm_writer.asmPush("sub esp, " + std::to_string(C_ALIGN(node->getBodyNode()->getBodySize())));

    //add body generation


    m_asm_writer.asmPush("add esp, " + std::to_string(C_ALIGN(node->getBodyNode()->getBodySize())));
  }
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
  std::string var_name = node->getStringValue();
  std::string var_value = "0";
  std::shared_ptr < datatype > datatype = node->getDatatype();
  if (node->getValueNode())
  {
    var_value = std::to_string(node->getValueNode()->getNumberValue());
  }
  m_asm_writer.asmPush(var_name + ": " + datatype->getStringForPrimitiveSize() + " " + var_value);
}
