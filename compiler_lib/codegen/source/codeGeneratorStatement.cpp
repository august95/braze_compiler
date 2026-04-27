#include "../codeGeneratorStatement.h"
#include "../codeGeneratorExpression.h"
#include "../codeGeneratorGlobalDecleration.h"
#include "../codeGenerator.h"

#define C_STACK_ALIGNMENT 16
#define STACK_PUSH_SIZE 4
#define C_ALIGN(size) (size % C_STACK_ALIGNMENT) ? size + (C_STACK_ALIGNMENT - (size % C_STACK_ALIGNMENT)) : size

codeGeneratorStatement::codeGeneratorStatement(asmWriter& asm_writer, resolver& resolver, codeGeneratorExpression* codegen_expression, class codeGeneratorGlobalDecleration* codegen_global_declaration, codeGenerator* codegen)
  :m_asm_writer(asm_writer),
  m_resolver(resolver),
  m_codegen_expression(codegen_expression),
  m_codegen_global_declaration(codegen_global_declaration),
  m_codegen(codegen)
{
}

void codeGeneratorStatement::generateStatement(std::shared_ptr<node> node)
{

  if (node->getNodeType() == NODE_TYPE_VARIABLE)
  {
    generateScopedVariable(cast_node<nodeVariableDeclaration>(node));
  }
  else if (node->getNodeType() == NODE_TYPE_EXPRESSION)
  {
    m_codegen_expression->generateExpNode(cast_node<nodeExpression>(node));
  }
  else if (node->getNodeType() == NODE_TYPE_STATEMENT_IF)
  {
    generateStatementIf(cast_node<nodeStatement>(node));
  }
  else if (node->getNodeType() == NODE_TYPE_STATEMENT_WHILE)
  {
    generateStatementWhile(cast_node<nodeStatement>(node));
  }
  else if (node->getNodeType() == NODE_TYPE_STATEMENT_FOR)
  {
    generateStatementFor(cast_node<nodeStatement>(node));
  }
  else if (node->getNodeType() == NODE_TYPE_STATEMENT_CONTINUE)
  {
    generateStatementContinue(cast_node<nodeStatement>(node));
  }
  else if (node->getNodeType() == NODE_TYPE_STATEMENT_BREAK)
  {
    generateStatementBreak(cast_node<nodeStatement>(node));
  }
  else if (node->getNodeType() == NODE_TYPE_UNARY)
  {
    m_codegen_expression->generateUnary(cast_node<nodeExpression>(node));
  }
  m_asm_writer.discardUnusedStack();
}

void codeGeneratorStatement::generateStatementFor(std::shared_ptr<nodeStatement> node)
{
  int for_loop_start = m_codegen->generateLableCount();
  int for_loop_end = m_codegen->generateLableCount();
  if (node->getInitNode())
  {
    generateScopedVariable(node->getInitNode());
    // m_asm_writer.asmGenPopIns("eax");
  }

  m_asm_writer.asmGen("jmp .for_loop" + std::to_string(for_loop_start));
  generateEntryPoint();
  generateExitPoint();
  if (node->getLoopNode())
  {
    m_codegen_expression->generateExpressionable(cast_node<nodeExpression>(node->getLoopNode()), 0);
    // m_asm_writer.asmGenPopIns("eax");
  }
  m_asm_writer.asmGen(".for_loop" + std::to_string(for_loop_start) + ":");
  if (node->getConditionNode())
  {
    m_codegen_expression->generateExpressionable(node->getConditionNode(), 0);
    m_asm_writer.asmGenPopIns("eax");
    m_asm_writer.asmGen("cmp eax, 0");
    m_asm_writer.asmGen("je .for_loop_end" + std::to_string(for_loop_end));
  }

  if (node->getBodyNode())
  {
    m_codegen_global_declaration->generateBody(node->getBodyNode());
  }

  if (node->getLoopNode())
  {
    m_codegen_expression->generateExpressionable(cast_node<nodeExpression>(node->getLoopNode()), 0);
    // m_asm_writer.asmGenPopIns("eax");
  }

  m_asm_writer.asmGen("jmp .for_loop" + std::to_string(for_loop_start));
  m_asm_writer.asmGen(".for_loop_end" + std::to_string(for_loop_end) + ":");
  finishEntryPoint();
  finishExitPoint();
}

void codeGeneratorStatement::generateStatementWhile(std::shared_ptr<nodeStatement> node)
{
  generateEntryPoint();
  generateExitPoint();
  int while_start = m_codegen->generateLableCount();
  int while_end = m_codegen->generateLableCount();

  m_asm_writer.asmGen(".while_start_" + std::to_string(while_start) + ":");
  m_codegen_expression->generateExpressionable(cast_node<nodeExpression>(node->getConditionNode()), 0);
  m_asm_writer.asmGenPopIns("eax");
  m_asm_writer.asmGen("cmp eax, 0");
  m_asm_writer.asmGen("je .while_end_" + std::to_string(while_end));
  m_codegen_global_declaration->generateBody(node->getBodyNode());
  m_asm_writer.asmGen("jmp .while_start_" + std::to_string(while_start));
  m_asm_writer.asmGen(".while_end_" + std::to_string(while_end) + ":");

  finishEntryPoint();
  finishExitPoint();
}

void codeGeneratorStatement::generateStatementIf(std::shared_ptr<nodeStatement> node)
{
  int end_label = m_codegen->generateLableCount();
  generateStatementIf_(node, end_label);

  m_asm_writer.asmGen(".if_end_" + std::to_string(end_label) + ":");
}

void codeGeneratorStatement::generateStatementIf_(std::shared_ptr<nodeStatement> node, int end_label)
{
  int if_label = m_codegen->generateLableCount();

  m_codegen_expression->generateExpressionable(cast_node<nodeExpression>(node->getConditionNode()), 0);
  m_asm_writer.asmGenPopIns("eax");
  m_asm_writer.asmGen("cmp eax, 0");                         // if equal, sets zero flag in CPU
  m_asm_writer.asmGen("je .if_" + std::to_string(if_label)); // if zero flag is set in CPU, perfores jump. We dont want to jump when if(0)
  m_codegen_global_declaration->generateBody(node->getBodyNode());
  m_asm_writer.asmGen("jmp .if_end_" + std::to_string(end_label));
  m_asm_writer.asmGen(".if_" + std::to_string(if_label) + ":");

  if (node->getNextElseNode())
  {
    generateStatementIfElse(node->getNextElseNode(), end_label);
  }
}

void codeGeneratorStatement::generateStatementIfElse(std::shared_ptr<nodeStatement> node, int end_label)
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

void codeGeneratorStatement::generateStatementElse(std::shared_ptr<nodeStatement> node, int end_label)
{
  m_codegen_global_declaration->generateBody(node->getBodyNode());
}

void codeGeneratorStatement::generateScopedVariable(std::shared_ptr<nodeVariableDeclaration> node)
{

  std::shared_ptr<resolverEntity> entity = m_resolver.addEntity(node, true);
  if (node->getValueNode())
  {
    m_codegen_expression->generateValueNode(node, entity);
  }

}


void codeGeneratorStatement::generateStatementBreak(std::shared_ptr<nodeStatement> node)
{
  if (node->getNodeType() != NODE_TYPE_STATEMENT_BREAK)
  {
    cerror("expected break node!");
    assert(0);
  }
  if (m_exit_point_stack.empty())
  {
    cerror("no place to break!");
    assert(0);
  }
  m_asm_writer.asmGen("jmp " + m_exit_point_stack.back().m_label);
}


void codeGeneratorStatement::generateStatementContinue(std::shared_ptr<nodeStatement> node)
{
  if (node->getNodeType() != NODE_TYPE_STATEMENT_CONTINUE)
  {
    cerror("expected continue node!");
    assert(0);
  }
  if (m_entry_point_stack.empty())
  {
    cerror("no place to continue!");
    assert(0);
  }
  m_asm_writer.asmGen("jmp " + m_entry_point_stack.back().m_label);
}

void codeGeneratorStatement::generateEntryPoint()
{
  m_entry_point_stack.push_back(entryPoint(m_codegen->generateLableCount()));
  m_asm_writer.asmGen(m_entry_point_stack.back().m_label+":");
}

void codeGeneratorStatement::finishEntryPoint()
{
  m_entry_point_stack.pop_back();
}

void codeGeneratorStatement::generateExitPoint()
{
  m_exit_point_stack.push_back(exitPoint(m_codegen->generateLableCount()));
}

void codeGeneratorStatement::finishExitPoint()
{
  m_asm_writer.asmGen(m_exit_point_stack.back().m_label+":");
  m_exit_point_stack.pop_back();
}
