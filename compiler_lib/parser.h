#pragma once

// receives a list of tokens, creates an abstract syntax tree with nodes

#include "token.h"
#include "node_/node.h"
#include "node_/nodeExpression.h"
#include "datatype.h"
#include "scope.h"
#include "symbolResolver.h"
#include <list>
#include <iostream>
#include <memory>

/*
 * FIXME:
 * Add parsing of unary operator in parseNormalExpression.
 * Add rules for parsing expressions with [, (, ?, ','. And reorder expression if needed.*
 */

class parser
{
public:
  parser();

  void setTokenList(std::list<std::shared_ptr<token>> tokens) { m_tokens = tokens; };
  int startParser();
  std::list<std::shared_ptr<node>> getAbstractSyntaxTree() { return m_nodes; }
  std::shared_ptr<scope> lastScope() { return m_last_scope; }

private:
  std::shared_ptr<token> nextToken();
  void pushToken(std::shared_ptr<token> token);
  std::shared_ptr<token> peekToken();
  void pushNode(std::shared_ptr<node> node);
  std::shared_ptr<node> peekLastNode();
  std::shared_ptr<node> peekLastNodeExpect(nodeType node_type);
  std::shared_ptr<node> popLastNode();
  std::shared_ptr<nodeExpression> makeExpressionNode(filePosition file_position, std::string operator_, std::shared_ptr<nodeExpression> left_node, std::shared_ptr<nodeExpression> right_node);
  std::shared_ptr<scope> newScope();
  void addNodeToCurrentScope(std::shared_ptr<node> node);
  void finishScope();



  void parseTokens();
  void parseNextToken();

  void parseExpression();
  void parseExpressionOperatorOrOperand(bool& continue_to_parse_exp);
  void parseOperand();
  void parseOperator();
  void parseParenthesesExpressionOrFunctionCall();
  void parseComma();
  void parseNormalExpression();
  void parseIncrementOperator();
  void parseKeyword();
  void parseWhileStatement();
  void parseForStatement();
  void parseIfStatement();
  void parseElseIfOrElseStatement();
  void parseGlobalKeyword();
  void parseVariableOrFunction();
  void parseFunction();
  void parseBody();
  void parseStatement();
  void parseFunctionParameters();
  void parseSymbol();
  void parseUnary();
  void parseNormalUnary();
  void parseIndirectionUnary();

  std::shared_ptr<datatype> parseDatatype();

private:
  /**/
  std::list<std::shared_ptr<node>> m_nodes;
  std::list<std::shared_ptr<token>> m_tokens;
  std::shared_ptr<scope> m_root_scope;
  std::shared_ptr<scope> m_last_scope;

  symbolResolver m_symbol_resolver;

  void _assert_(bool condition, std::string message);
};
