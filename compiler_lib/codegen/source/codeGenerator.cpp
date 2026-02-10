#include "../../source/pch.h"
#include "../codeGenerator.h"
#include "../../braze_compiler.h"
#include "../resolver/resolverResult.h"

#define C_STACK_ALIGNMENT 16
#define STACK_PUSH_SIZE 4
#define C_ALIGN(size) (size % C_STACK_ALIGNMENT) ? size + (C_STACK_ALIGNMENT - (size % C_STACK_ALIGNMENT)) : size

codeGenerator::codeGenerator()
  : m_codegen_expression(m_asm_writer, m_resolver)
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

void codeGenerator::setFileName(std::string filename, asmWriter::WriteMode write_mode)
{
  m_input_file = filename;
  output_file = filename + ".asm";
  m_asm_writer.setWriteMode(write_mode);
}

int codeGenerator::startCodeGeneration()
{

  initialize();

  m_root_scope.init(std::make_shared<scope>(), true);

  generateDataSection();

  generateRoot();

  generateReadOnlySection();

  m_asm_writer.close();

  return 0;
}

void codeGenerator::generateDataSection()
{
  m_asm_writer.asmGen("section .data");

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
  m_asm_writer.asmGen("section .text");

  for (auto node : m_ast)
  {
    if (node->getNodeType() == NODE_TYPE_VARIABLE)
    {
      // Global variables already processed in generateDataSection
    }
    if (node->getNodeType() == NODE_TYPE_FUNCTION)
    {
      generateFunction(node);
    }
  }
}

void codeGenerator::generateReadOnlySection()
{
  m_asm_writer.asmGen("section .rodata");
  m_codegen_expression.generateWriteStrings();
}

void codeGenerator::generateRootNode(std::shared_ptr<node> node)
{
  if (node->getNodeType() == NODE_TYPE_FUNCTION)
  {
    m_resolver.registerFunction(node);
    if (node->isFunctionPrototype())
    {
      m_asm_writer.asmGen("extern " + node->getStringValue());
    }
    else
    {
      // function declaration
      std::string function_name = node->getStringValue();
      m_asm_writer.asmGen("global " + function_name);
      m_asm_writer.asmGen(function_name + ":");

      bool has_stack_size = C_ALIGN(node->getBodyNode()->getBodySize()) != 0;
      m_asm_writer.asmGenPushEbp(C_ALIGN(node->getBodyNode()->getBodySize()));

      m_resolver.createNewScope(true, false);
      generateFunctionParameters(node);

      m_resolver.createNewScope(true, false);
      generateBody(node->getBodyNode());
      m_resolver.removeScope();

      m_resolver.removeScope();
      m_asm_writer.asmGenPopEbp(C_ALIGN(node->getBodyNode()->getBodySize()));
    }
  }
}

void codeGenerator::generateFunction(std::shared_ptr<node> node)
{
  // deal with forward declaration
  generateRootNode(node);
}

void codeGenerator::generateFunctionParameters(std::shared_ptr<node> node_)
{
  std::list<std::shared_ptr<node>> function_arguemnt = node_->getFunctionArguments();
  for (auto it = function_arguemnt.begin(); it != function_arguemnt.end(); ++it)
  {
    std::shared_ptr<node> argument_node = (*it);
    std::shared_ptr<resolverEntity> entity = m_resolver.addEntity(argument_node, true);
  }
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
  }
  else if (node->getNodeType() == NODE_TYPE_EXPRESSION)
  {
    m_codegen_expression.generateExpNode(cast_node<nodeExpression>(node));
  }
  else if (node->getNodeType() == NODE_TYPE_STATEMENT_IF)
  {
    generateStatementIf(node);
  }
  else if (node->getNodeType() == NODE_TYPE_STATEMENT_WHILE)
  {
    generateStatementWhile(node);
  }
  else if (node->getNodeType() == NODE_TYPE_STATEMENT_FOR)
  {
    generateStatementFor(node);
  }
  else if (node->getNodeType() == NODE_TYPE_UNARY)
  {
    m_codegen_expression.generateUnary(cast_node<nodeExpression>(node));
  }
  m_asm_writer.discardUnusedStack();
}

void codeGenerator::generateStatementFor(std::shared_ptr<node> node)
{
  int for_loop_start = generateLableCount();
  int for_loop_end = generateLableCount();
  if (node->getInitNode())
  {
    generateScopedVariable(node->getInitNode());
    // m_asm_writer.asmGenPopIns("eax");
  }

  m_asm_writer.asmGen("jmp .for_loop" + std::to_string(for_loop_start));
  if (node->getLoopNode())
  {
    m_codegen_expression.generateExpressionable(cast_node<nodeExpression>(node->getLoopNode()), 0);
    // m_asm_writer.asmGenPopIns("eax");
  }
  m_asm_writer.asmGen(".for_loop" + std::to_string(for_loop_start) + ":");
  if (node->getConditionNode())
  {
    m_codegen_expression.generateExpressionable(cast_node<nodeExpression>(node->getConditionNode()), 0);
    m_asm_writer.asmGenPopIns("eax");
    m_asm_writer.asmGen("cmp eax, 0");
    m_asm_writer.asmGen("je .for_loop_end" + std::to_string(for_loop_end));
  }

  if (node->getBodyNode())
  {
    generateBody(node->getBodyNode());
  }

  if (node->getLoopNode())
  {
    m_codegen_expression.generateExpressionable(cast_node<nodeExpression>(node->getLoopNode()), 0);
    // m_asm_writer.asmGenPopIns("eax");
  }

  m_asm_writer.asmGen("jmp .for_loop" + std::to_string(for_loop_start));
  m_asm_writer.asmGen(".for_loop_end" + std::to_string(for_loop_end) + ":");
}

void codeGenerator::generateStatementWhile(std::shared_ptr<node> node)
{
  int while_start = generateLableCount();
  int while_end = generateLableCount();

  m_asm_writer.asmGen(".while_start_" + std::to_string(while_start) + ":");
  m_codegen_expression.generateExpressionable(cast_node<nodeExpression>(node->getConditionNode()), 0);
  m_asm_writer.asmGenPopIns("eax");
  m_asm_writer.asmGen("cmp eax, 0");
  m_asm_writer.asmGen("je .while_end_" + std::to_string(while_end));
  generateBody(node->getBodyNode());
  m_asm_writer.asmGen("jmp .while_start_" + std::to_string(while_start));
  m_asm_writer.asmGen(".while_end_" + std::to_string(while_end) + ":");
}

void codeGenerator::generateStatementIf(std::shared_ptr<node> node)
{
  int end_label = generateLableCount();
  generateStatementIf_(node, end_label);

  m_asm_writer.asmGen(".if_end_" + std::to_string(end_label) + ":");
}

void codeGenerator::generateStatementIf_(std::shared_ptr<node> node, int end_label)
{
  int if_label = generateLableCount();

  m_codegen_expression.generateExpressionable(cast_node<nodeExpression>(node->getConditionNode()), 0);
  m_asm_writer.asmGenPopIns("eax");
  m_asm_writer.asmGen("cmp eax, 0");                         // if equal, sets zero flag in CPU
  m_asm_writer.asmGen("je .if_" + std::to_string(if_label)); // if zero flag is set in CPU, perfores jump. We dont want to jump when if(0)
  generateBody(node->getBodyNode());
  m_asm_writer.asmGen("jmp .if_end_" + std::to_string(end_label));
  m_asm_writer.asmGen(".if_" + std::to_string(if_label) + ":");

  if (node->getNextElseNode())
  {
    generateStatementIfElse(node->getNextElseNode(), end_label);
  }
}

void codeGenerator::generateStatementIfElse(std::shared_ptr<node> node, int end_label)
{
  if (node->getNodeType() == NODE_TYPE_STATEMENT_IF)
  {
    generateStatementIf_(node, end_label);
  }
  else if (node->getNodeType() == NODE_TYPE_STATEMENT_ELSE)
  {
    generateStatementElse(node, end_label);
  }
  else
  {
    assert(0);
  }
}

void codeGenerator::generateStatementElse(std::shared_ptr<node> node, int end_label)
{
  generateBody(node->getBodyNode());
}

void codeGenerator::generateGlobalVariable(std::shared_ptr<node> node)
{
  if (node)
  {
    if (node->getNodeType() == NODE_TYPE_VARIABLE)
    {
      std::shared_ptr<datatype> datatype = node->getDatatype();

      // do not deal with structs and unions here!
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
  std::shared_ptr<resolverEntity> entity = m_resolver.addEntity(node, false);

  std::string var_name = node->getStringValue();
  std::string var_value = "0";
  std::shared_ptr<datatype> datatype = node->getDatatype();
  if (std::shared_ptr<nodeExpression> node_exp = cast_node<nodeExpression>(node->getValueNode()))
  {
    var_value = std::to_string(node_exp->getNumberValue());
  }
  m_asm_writer.asmGen(var_name + ": " + datatype->getStringForPrimitiveSize() + " " + var_value);
}

void codeGenerator::generateScopedVariable(std::shared_ptr<node> node)
{
  if (node->getNodeType() == NODE_TYPE_VARIABLE)
  {
    std::shared_ptr<resolverEntity> entity = m_resolver.addEntity(node, true);
    if (node->getValueNode())
    {
      m_codegen_expression.generateValueNode(cast_node<nodeExpression>(node), entity);
    }
  }
}

int codeGenerator::generateLableCount(bool reset)
{
  static int count = 0;
  count++;

  // static counter lives during all unit tests, needs to be cleared after each tests
  if (reset)
  {
    count = 0;
  }
  return count;
  m_codegen_expression.generateLableCount(reset);
}
