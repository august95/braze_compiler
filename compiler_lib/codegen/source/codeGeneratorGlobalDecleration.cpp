#include "../codeGeneratorGlobalDecleration.h"
#include "../codeGeneratorStatement.h"
#include "../codeGeneratorExpression.h"
#include "../codeGenerator.h"


codeGeneratorGlobalDecleration::codeGeneratorGlobalDecleration(asmWriter& asm_writer, resolver& resolver, codeGeneratorStatement* codegen_statement, codeGeneratorExpression* codegen_expression, codeGenerator* codegen)
  :m_asm_writer(asm_writer),
  m_resolver(resolver),
  m_codegen_statement(codegen_statement),
  m_codegen_expression(codegen_expression),
  m_codegen(codegen)
{
}


void codeGeneratorGlobalDecleration::generateFunctionDeclaration(std::shared_ptr<nodeFunctionDeclaration> node)
{
  if (node->isFunctionPrototype())
  {
    generateFunctionPrototpe(node);
    return;
  }
  generateFunctionDeclaration_(node);
}

void codeGeneratorGlobalDecleration::generateFunctionDeclaration_(std::shared_ptr<nodeFunctionDeclaration> node)
{
  m_current_function = node;
  m_resolver.registerFunction(node);
  std::string function_name = node->getStringValue();
  m_asm_writer.asmGen("global " + function_name);
  m_asm_writer.asmGen(function_name + ":");
  m_asm_writer.asmGenPushEbp(C_ALIGN(node->getBodyNode()->getBodySize()));

  m_resolver.createNewScope(true, false);
  generateFunctionParameters(node);

  m_resolver.createNewScope(true, false);
  generateBody(cast_node<nodeBody>(node->getBodyNode()));
  m_resolver.removeScope();

  m_resolver.removeScope(); //function parameters

  m_asm_writer.asmGenPopEbp(C_ALIGN(node->getBodyNode()->getBodySize()));  
  m_current_function = 0;
}

void codeGeneratorGlobalDecleration::generateFunctionPrototpe(std::shared_ptr<nodeFunctionDeclaration> node)
{
  m_resolver.registerFunction(node);
  m_asm_writer.asmGen("extern " + node->getStringValue());
}

void codeGeneratorGlobalDecleration::generateFunctionParameters(std::shared_ptr<nodeFunctionDeclaration> node_)
{
  std::list<std::shared_ptr<node>> function_arguemnt = node_->getFunctionArguments();
  for (auto it = function_arguemnt.begin(); it != function_arguemnt.end(); ++it)
  {
    std::shared_ptr<node> argument_node = (*it);
    std::shared_ptr<resolverEntity> entity = m_resolver.registerVariable(argument_node);
  }
}

void codeGeneratorGlobalDecleration::generateBody(std::shared_ptr<nodeBody> node)
{
  m_resolver.createNewScope(false, true);
  generateScope(node);
  m_resolver.removeScope();
}

void codeGeneratorGlobalDecleration::generateScope(std::shared_ptr<nodeBody> node)
{
  if (node)
  {
    if (node->getNodeType() == NODE_TYPE_BODY)
    {
      for (auto statement : node->getStatements())
      {
        m_codegen_statement->generateStatement(statement);
      }
    }
  }
}


void codeGeneratorGlobalDecleration::generateGlobalVariable(std::shared_ptr<nodeVariableDeclaration> node)
{
  if (node)
  {
    std::shared_ptr<datatype> datatype = node->getDatatype();
    // do not deal with structs and unions here!
    if (datatype->isPrimitiveType())
    {
      generateGlobalVariablePrimitive(node);
    }

  }
}

void codeGeneratorGlobalDecleration::generateGlobalVariablePrimitive(std::shared_ptr<nodeVariableDeclaration> node)
{
  if (node->getNodeType() == NODE_TYPE_STRING)
  {
    cerror("Codegen: strings not yet implemented");
    return;
  }
  std::shared_ptr<resolverEntity> entity = m_resolver.registerVariable(node);

  std::string var_name = node->getStringValue();
  std::string var_value = "0";
  std::shared_ptr<datatype> datatype = node->getDatatype();
  if (std::shared_ptr<nodeExpression> node_exp = cast_node<nodeExpression>(node->getValueNode()))
  {
    var_value = std::to_string(node_exp->getNumberValue());
  }
  m_asm_writer.asmGen(var_name + ": " + datatype->getStringForPrimitiveSize() + " " + var_value);
}

