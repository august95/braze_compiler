#include "pch.h"
#include "../parser.h"
#include "../braze_compiler.h"
#include "../precedenceHandler.h"
#include "../node/nodeExpression.h"

#include <memory>

parser::parser()
  : m_root_scope(std::make_shared<scope>())
{
  m_root_scope->init(std::shared_ptr<scope>(), true);
  m_last_scope = m_root_scope;
}

int parser::startParser()
{
  parseTokens();

  if (!m_nodes.empty())
  {
    int stack_offset = 0;
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it)
    {
      std::shared_ptr<node> current_node = *it;
      current_node->calculateStackOffset(stack_offset);
    }
    return 0;
  }

  return -1;
}

std::shared_ptr<token> parser::nextToken()
{
  if (m_tokens.empty())
    return std::make_shared<token>();

  std::shared_ptr<token> token = m_tokens.front();
  m_tokens.pop_front();

  // ignore new line tokens
  while (!m_tokens.empty() && token->isTokenTypeNewLine())
  {
    token = m_tokens.front();
    m_tokens.pop_front();
  }
  return token;
}

void parser::pushToken(std::shared_ptr<token> token)
{
  m_tokens.push_front(token);
}

std::shared_ptr<token> parser::peekToken()
{
  if (m_tokens.empty())
    return std::make_shared<token>();

  // ignore new line tokens
  while (!m_tokens.empty() && m_tokens.front()->isTokenTypeNewLine())
  {
    m_tokens.pop_front();
  }
  // the loop might have emptied the last token
  if (m_tokens.empty())
    return std::make_shared<token>();
  return m_tokens.front();
}

void parser::addNodeToCurrentScope(std::shared_ptr<node> node)
{
  m_last_scope->addNode(node);
}

void parser::pushNode(std::shared_ptr<node> node)
{
  m_nodes.push_back(node);
}

std::shared_ptr<node> parser::peekLastNode()
{
  if (!m_nodes.empty())
    return m_nodes.back();
  return std::shared_ptr<node>();
}

std::shared_ptr<node> parser::peekLastNodeExpect(nodeType node_type)
{
  std::shared_ptr<node> node = peekLastNode();
  _assert_(node->getNodeType() & node_type, "expected other node type");
  return node;
}
std::shared_ptr<node> parser::popLastNode()
{
  auto node = m_nodes.back();
  m_nodes.pop_back();
  return node;
}
std::shared_ptr<nodeExpression> parser::makeExpressionNode(filePosition file_position, std::string operator_, std::shared_ptr<nodeExpression> left_node, std::shared_ptr<nodeExpression> right_node)
{

  std::shared_ptr<nodeExpression> expression_node = std::make_shared<nodeExpression>(NODE_TYPE_EXPRESSION, file_position);
  expression_node->setLeftNode(left_node);
  expression_node->setRightNode(right_node);
  expression_node->setStringValue(operator_);
  expression_node->generateExpressionFlag();
  return expression_node;
}



void parser::parseTokens()
{
  while (!m_tokens.empty())
  {
    parseNextToken();
  }
}

void parser::parseNextToken()
{
  std::shared_ptr<token> token = peekToken();
  if (token->isTokenTypeNumber() || token->isTokenTypeIdentifier() || token->isTokenTypeString())
  {
    parseExpression();
  }
  if (token->isTokenTypeKeyword())
  {
    parseGlobalKeyword();
  }
}

void parser::parseExpression()
{
  // parse all the tokens in the expression until the ';'. Example :a = b + 5;
  bool continue_to_parse_exp = false;
  do
  {

    parseExpressionOperatorOrOperand(continue_to_parse_exp);
  } while (continue_to_parse_exp);
}

void parser::parseExpressionOperatorOrOperand(bool& continue_to_parse_exp)
{
  std::shared_ptr<token> token = peekToken();
  if (!token)
  {
    continue_to_parse_exp = false;
    return;
  }
  if (token->isTokenTypeNumber() || token->isTokenTypeIdentifier() || token->isTokenTypeString())
  {
    continue_to_parse_exp = true;
    parseOperand();
    return;
  }
  else if (token->isTokenTypeOperator())
  {
    continue_to_parse_exp = true;
    parseOperator();
    return;
  }
  // TODO: add keyword '(' in case of cast:  int a = (int)b;

  continue_to_parse_exp = false;
}

void parser::parseOperand()
{
  std::shared_ptr<token> token = nextToken();
  std::shared_ptr<nodeExpression> node_;

  if (token->isTokenTypeChar())
  {
    // we want the char to be handled as a number in expressions and precedence handling, int a[e]; is allowed
    node_ = std::make_shared<nodeExpression>(nodeType::NODE_TYPE_NUMBER, token->getFilePosition());
    std::shared_ptr<datatype> datatype_ = std::make_shared<datatype>(token->getFilePosition());
    datatype_->setDataType("char");
    datatype_->setRValue(true);
    node_->setDatatype(datatype_);
    node_->setNumberValue(token->getNumberValue());
  }
  else if (token->isTokenTypeNumber())
  {
    node_ = std::make_shared<nodeExpression>(nodeType::NODE_TYPE_NUMBER, token->getFilePosition());
    std::shared_ptr<datatype> datatype_ = std::make_shared<datatype>(token->getFilePosition());
    datatype_->setDataType("int");
    datatype_->setRValue(true);
    node_->setDatatype(datatype_);
    node_->setNumberValue(token->getNumberValue());
  }
  else if (token->isTokenTypeIdentifier())
  {
    node_ = std::make_shared<nodeExpression>(nodeType::NODE_TYPE_IDENTIFIER, token->getFilePosition());
    node_->setStringValue(token->getStringValue());
    node_->setDeclarationNode(m_symbol_resolver.findDeclerationNode(node_));
    node_->setDatatype(node_->getDeclarationNode() ? node_->getDeclarationNode()->getDatatype() : 0);
  }
  else if (token->isTokenTypeString())
  {
    node_ = std::make_shared<nodeExpression>(nodeType::NODE_TYPE_STRING, token->getFilePosition());
    std::shared_ptr<datatype> datatype_ = std::make_shared<datatype>(token->getFilePosition());
    datatype_->setDataType("__internal_only_string__");
    datatype_->setRValue(true);
    node_->setDatatype(datatype_);
    node_->setStringValue(token->getStringValue());
  }
  pushNode(node_);
}

void parser::parseOperator()
{
  std::shared_ptr<token> operatort_token = peekToken();
  if (STRINGS_EQUAL(operatort_token->getStringValue().c_str(), "("))
  {
    parseParenthesesExpressionOrFunctionCall();
    return;
  }
  else if (STRINGS_EQUAL(operatort_token->getStringValue().c_str(), ","))
  {
    parseComma();
    return;
  }
  parseNormalExpression();
}

void parser::parseParenthesesExpressionOrFunctionCall()
{
  std::shared_ptr<token> token = nextToken();
  assert(STRINGS_EQUAL(token->getStringValue().c_str(), "("));
  token = peekToken();
  if (token->isTokenTypeKeyword())
  {
    // (int)
    // FIXME: handle cast
  }
  std::shared_ptr<nodeExpression> function_call;
  // true for test(50+30) function call
  //  50 + (30 + 20 is not capured here because of the operator
  if (cast_node<nodeExpression>(peekLastNode())->isValueNode())
  {
    function_call = cast_node<nodeExpression>(popLastNode());
  }
  std::shared_ptr<nodeExpression> expression_node;
  token = peekToken();
  if (token->getCharValue() != ')')
  {
    // we have content between '(' & ')'
    parseExpression();
    expression_node = cast_node<nodeExpression>(popLastNode()); ;
  }
  token = nextToken();
  assert(token->getCharValue() == ')');

  std::shared_ptr<nodeExpression> parentheses_node = std::make_shared<nodeExpression>(NODE_TYPE_EXPRESSION_PARANTHESES, token->getFilePosition());
  parentheses_node->setParenthesesNode(expression_node);
  if (function_call)
  {
    // having function calls as expression type makes precedence much more easer!!! function calls is recognized by having "()" as op
    std::shared_ptr<nodeExpression> node_ = std::make_shared<nodeExpression>(NODE_TYPE_EXPRESSION, token->getFilePosition());
    node_->setLeftNode(function_call);
    node_->setRightNode(parentheses_node);
    node_->setStringValue("()");
    pushNode(node_);
  }
  else
  {
    pushNode(parentheses_node);
  }

  // the expression continues:
  //  50 + func(50) * 30;
  //  50 + (30 +20) - 4
  token = peekToken();
  if (token->isTokenTypeOperator())
  {
    parseExpression();
  }
}

void parser::parseComma()
{
  std::shared_ptr<token> operatort_token = nextToken();
  assert(STRINGS_EQUAL(operatort_token->getStringValue().c_str(), ","));
  std::shared_ptr<nodeExpression> left_node = cast_node<nodeExpression>(popLastNode());
  parseExpression();
  std::shared_ptr<nodeExpression> right_node = cast_node<nodeExpression>(popLastNode());
  pushNode(makeExpressionNode(operatort_token->getFilePosition(), ",", left_node, right_node));
}

void parser::parseNormalExpression()
{
  // 50 * 30 + 20
  std::shared_ptr<nodeExpression> left_node = cast_node<nodeExpression>(peekLastNode());    // 50
  std::shared_ptr<token> operator_token = peekToken(); // *
  // std::string operator_ = operatort_token->getStringValue();

  if (!left_node)
  {
    if (operator_token->isUnaryOperator())
    {
      parseUnary();
    }
    else
    {
      cwarning("expression has no left operand, expected unary, but no found", operator_token->getFilePosition());
    }
    return;
  }
  if (STRINGS_EQUAL(operator_token->getStringValue().c_str(), "++") ||
    STRINGS_EQUAL(operator_token->getStringValue().c_str(), "--"))
  {
    parseIncrementOperator();
    return;
  }
  nextToken();                    // operator token popped '*'
  popLastNode();                    // 50
  parseExpression();                  // parse 30 + 20
  std::shared_ptr<nodeExpression> right_node = cast_node<nodeExpression>(popLastNode()); // + L(30) R(20)
  std::shared_ptr<nodeExpression> expression_node = cast_node<nodeExpression>(makeExpressionNode(operator_token->getFilePosition(), operator_token->getStringValue(), left_node, right_node));

  precedenceHandler::reorderExpression(expression_node);

  pushNode(expression_node);
}

void parser::parseIncrementOperator()
{
  //FIXME: create new node type and let code gen handle increment operator.
  // in case of array the result needs to be pushed to stack, in order to access array X element
  //For now to make GUI work, create following node structure 
  /*
      =
    i   +/-
      i    1
  */

  std::shared_ptr<nodeExpression> left_node = cast_node< nodeExpression>(popLastNode());    // i
  std::shared_ptr<token> operator_token = nextToken(); // ++

  //1
  std::shared_ptr<nodeExpression> node_1 = std::make_shared<nodeExpression>(nodeType::NODE_TYPE_NUMBER, left_node->getFilePosition());
  std::shared_ptr<datatype> datatype_ = std::make_shared<datatype>(left_node->getFilePosition());
  datatype_->setDataType("int");
  datatype_->setRValue(true);
  node_1->setDatatype(datatype_);
  node_1->setNumberValue(1);

  //i
  std::shared_ptr<nodeExpression> node_i = std::make_shared<nodeExpression>();
  *node_i = *left_node;


  //+ or -
  std::shared_ptr<nodeExpression> add_or_subtract_node;
  if (STRINGS_EQUAL(operator_token->getStringValue().c_str(), "++"))
    add_or_subtract_node = makeExpressionNode(operator_token->getFilePosition(), "+", node_i, node_1);
  else if (STRINGS_EQUAL(operator_token->getStringValue().c_str(), "--"))
    add_or_subtract_node = makeExpressionNode(operator_token->getFilePosition(), "-", node_i, node_1);


  std::shared_ptr<nodeExpression> assignment_node = makeExpressionNode(operator_token->getFilePosition(), "=", left_node, add_or_subtract_node);
  pushNode(assignment_node);
}

void parser::parseKeyword()
{
  std::shared_ptr<token> token = peekToken();

  if (datatype::isKeywordVariableModifier(token->getStringValue()) || datatype::IsKeywordDatatype(token->getStringValue()))
  {
    // static const int func_or_variable ...
    parseVariableOrFunction();
  }
  else if (STRINGS_EQUAL(token->getStringValue().c_str(), "if"))
  {
    parseIfStatement();
  }
  else if (STRINGS_EQUAL(token->getStringValue().c_str(), "while"))
  {
    parseWhileStatement();
  }
  else if (STRINGS_EQUAL(token->getStringValue().c_str(), "for"))
  {
    parseForStatement();
  }
}

void parser::parseWhileStatement()
{
  std::shared_ptr<token> token = nextToken();
  assert(STRINGS_EQUAL(token->getStringValue().c_str(), "while"));
  token = nextToken();
  assert(STRINGS_EQUAL(token->getStringValue().c_str(), "("));
  std::shared_ptr<node> while_node = std::make_shared<node>(nodeType::NODE_TYPE_STATEMENT_WHILE, token->getFilePosition());
  std::shared_ptr<node> condition_node;
  parseExpression();
  condition_node = popLastNode();
  while_node->setConditionNode(condition_node);
  token = nextToken();
  assert(token->getCharValue() == ')');
  std::shared_ptr<node> body_node;
  parseBody();
  body_node = popLastNode();
  while_node->setBodyNode(body_node);
  pushNode(while_node);
}

void parser::parseForStatement()
{
  // for (init; condition; loop)
  std::shared_ptr<token> token = nextToken();
  assert(STRINGS_EQUAL(token->getStringValue().c_str(), "for"));
  token = nextToken();
  assert(STRINGS_EQUAL(token->getStringValue().c_str(), "("));

  std::shared_ptr<node> for_node = std::make_shared<node>(nodeType::NODE_TYPE_STATEMENT_FOR, token->getFilePosition());

  std::shared_ptr<node> init_node;
  token = peekToken();
  if (token->isTokenTypeKeyword())
  {
    parseKeyword();
  }
  else
  {
    parseExpression();
    assert(token->getCharValue() == ';');
  }
  init_node = popLastNode();
  for_node->setInitNode(init_node);

  std::shared_ptr<node> condition_node;
  parseExpression();
  condition_node = popLastNode();
  for_node->setConditionNode(condition_node);
  token = nextToken();
  assert(token->getCharValue() == ';');

  std::shared_ptr<node> loop_node;
  parseExpression();
  loop_node = popLastNode();
  for_node->setLoopNode(loop_node);

  // body part
  token = nextToken();
  assert(token->getCharValue() == ')');
  std::shared_ptr<node> body_node;
  parseBody();
  body_node = popLastNode();
  for_node->setBodyNode(body_node);
  pushNode(for_node);
}

void parser::parseIfStatement()
{
  std::shared_ptr<token> token = nextToken();
  assert(STRINGS_EQUAL(token->getStringValue().c_str(), "if"));
  token = nextToken();
  assert(STRINGS_EQUAL(token->getStringValue().c_str(), "("));
  std::shared_ptr<node> if_node = std::make_shared<node>(nodeType::NODE_TYPE_STATEMENT_IF, token->getFilePosition());
  std::shared_ptr<node> condition_node;
  parseExpression();
  condition_node = popLastNode();
  token = nextToken();
  assert(token->getCharValue() == ')');
  std::shared_ptr<node> body_node;
  parseBody();
  body_node = popLastNode();
  if_node->setConditionNode(condition_node);
  if_node->setBodyNode(body_node);
  pushNode(if_node);

  parseElseIfOrElseStatement();
}

void parser::parseElseIfOrElseStatement()
{
  std::shared_ptr<node> if_node = peekLastNode();
  std::shared_ptr<token> token_ = peekToken();
  bool is_else = false;
  while (STRINGS_EQUAL(token_->getStringValue().c_str(), "else"))
  {
    token_ = nextToken();
    std::shared_ptr<token> token_next = peekToken();
    if (STRINGS_EQUAL(token_next->getStringValue().c_str(), "if"))
    {
      is_else = false;
      token_ = nextToken();
    }
    else
    {
      is_else = true;
    }

    std::shared_ptr<node> else_node = std::make_shared<node>(is_else ? NODE_TYPE_STATEMENT_ELSE : NODE_TYPE_STATEMENT_IF, token_->getFilePosition());
    std::shared_ptr<node> condition_node;
    if (!is_else)
    {
      token_ = nextToken(); // pop of '('
      parseExpression();
      condition_node = popLastNode();
      token_ = nextToken();
      assert(token_->getCharValue() == ')');
    }
    std::shared_ptr<node> body_node;
    parseBody();
    body_node = popLastNode();
    else_node->setConditionNode(condition_node);
    else_node->setBodyNode(body_node);
    if_node->setNextElseNode(else_node);
    if_node = else_node;
    token_ = peekToken();

    if (is_else == true)
    {
      break;
    }
  }
}

void parser::parseGlobalKeyword()
{
  parseKeyword();
  peekLastNode()->setIsGlobal(true);
}

void parser::parseVariableOrFunction()
{
  // to parse: static const long long*** var_name or  int function_name()

  std::shared_ptr<datatype> datatype = parseDatatype();

  if (datatype->isStruct() || datatype->isUnion())
  {
    // #warning "struct not implemented"
    // If global scope, parse struct declaration or function with struct return type
    // If inside statement, parse struct variable'
    //
  }

  std::shared_ptr<token> token = nextToken();
  assert(token->isTokenTypeIdentifier() && "expected variable name or function name");
  std::shared_ptr<nodeExpression> _node = std::make_shared<nodeExpression>(token->getFilePosition());
  std::string variable_or_function_name = token->getStringValue();
  _node->setStringValue(variable_or_function_name);

  token = nextToken();
  if (token->isTokenTypeSymbol() && (token->getCharValue() == ';'))
  {
    // unassigned variable int a;
    _node->setNodeType(nodeType::NODE_TYPE_VARIABLE);
    _node->setDatatype(datatype);
    m_symbol_resolver.addNodeToCurrentScope(_node);
  }
  else if (token->isTokenTypeOperator() && (token->getStringValue() == "="))
  {
    // assigned variable int a = 50;
    _node->setNodeType(nodeType::NODE_TYPE_VARIABLE);
    _node->setDatatype(datatype);
    parseExpression();
    _node->setValueNode(popLastNode());
    m_symbol_resolver.addNodeToCurrentScope(_node);
    nextToken(); // pop off ';'
  }
  else if (token->isTokenTypeOperator() && (token->getStringValue() == "("))
  {
    // parsing function int a(){}

    _node->setNodeType(nodeType::NODE_TYPE_FUNCTION);
    _node->setReturnDatatype(datatype);
    pushNode(_node); //_node is popped inside parseFunction
    parseFunction();

    m_symbol_resolver.addNodeToCurrentScope(peekLastNode());
    return; // function node already pushed
  }
  else
  {
    cerror("expected function or variable declaration", token->getFilePosition());
    assert(false);
  }
  pushNode(_node);
}

void parser::parseFunction()
{
  m_symbol_resolver.newScope();
  std::shared_ptr<node> function_node = peekLastNode();
  // deal with parameters
  std::shared_ptr<token> token = peekToken();
  m_symbol_resolver.newScope();
  if (token->getCharValue() != ')')
  {
    parseFunctionParameters();
  }
  else
  {
    nextToken(); // pop ')'
  }
  popLastNode(); // function_node
  token = peekToken();
  if (token->isTokenTypeSymbol() && token->getCharValue() == '{')
  {
    parseBody();
    std::shared_ptr<node> body_node = popLastNode();
    function_node->setBodyNode(body_node);
  }
  else
  {
    // pre declaration of function
    token = nextToken();
    assert(token->isTokenTypeSymbol() && token->getCharValue() == ';');
    function_node->setFunctionPrototype(true);
  }
  pushNode(function_node);
  m_symbol_resolver.finishScope();
  m_symbol_resolver.finishScope();
}

void parser::parseBody()
{
  m_symbol_resolver.newScope();
  // create new scope
  std::shared_ptr<token> token = nextToken(); // '{'
  std::list<std::shared_ptr<node>> statements;
  std::shared_ptr<node> body_node = std::make_shared<node>(nodeType::NODE_TYPE_BODY, token->getFilePosition());
  int stack_offset = 0;
  bool single_line_statement = false;

  //single line statement
  if (!token->isTokenTypeSymbol() || token->getCharValue() != '{')
  {
    pushToken(token); //'{' is not preset, push back token
    parseStatement();
    std::shared_ptr<node> statement_node = popLastNode();
    body_node->addStatement(statement_node);
    single_line_statement = true;
  }
  else if (!single_line_statement)
  {
    token = peekToken();
    while (!token->isTokenTypeSymbol() || (token->getCharValue() != '}'))
    {
      parseStatement();
      std::shared_ptr<node> statement_node = popLastNode();
      body_node->addStatement(statement_node);
      token = peekToken();
    }
    token = nextToken(); // pop off '}'(); // '}', parseGlobalKeyword will pop this symbol
  }

  if ((!token->isTokenTypeSymbol() || token->getCharValue() != '}') && !single_line_statement)
  {
    cerror("expected symbol '}' at ending of body", token->getFilePosition());
  }
  pushNode(body_node);
  m_symbol_resolver.finishScope();
}

void parser::parseStatement()
{
  std::shared_ptr<token> token = peekToken();
  if (token->isTokenTypeKeyword())
  {
    // variable, or struct
    parseKeyword();
    return;
  }

  parseExpression(); // if next token is symbol, no tokens are popped

  token = peekToken();

  if (token->isTokenTypeSymbol() && token->getCharValue() == '{')
  {
    parseSymbol();
    // nextToken(); //pop off '}'
    // Nested scope, return because ';' is not expected {{}}
    return;
  }
  token = nextToken();

  if ((!token->isTokenTypeSymbol()) || token->getCharValue() != ';')
  {
    cerror("expected ';' at ending of statement", token->getFilePosition());
  }
}

void parser::parseFunctionParameters()
{
  std::shared_ptr<node> function_node = peekLastNode();
  std::shared_ptr<token> token = peekToken();
  while (token->getCharValue() != ')')
  {
    std::shared_ptr<datatype> datatype = parseDatatype();
    if (datatype->isStruct() || datatype->isUnion())
    {
    }
    token = nextToken();
    if (STRINGS_EQUAL(token->getStringValue().c_str(), "..."))
    {
      nextToken(); // pop off ')'
      return;
    }
    assert(token->isTokenTypeIdentifier() && "expected variable name or function name");
    std::shared_ptr<nodeExpression> _node = std::make_shared<nodeExpression>(token->getFilePosition());
    _node->setStringValue(token->getStringValue());
    _node->setNodeType(NODE_TYPE_VARIABLE);
    _node->setIsFunctionArgument(true);
    _node->setDatatype(datatype);
    function_node->addFunctionArgumentNode(_node);
    m_symbol_resolver.addNodeToCurrentScope(_node);
    token = nextToken(); // pop of argument name

    if (STRINGS_EQUAL(token->getStringValue().c_str(), ","))
    {
      //      token = nextToken();
    }
  }
}

void parser::parseSymbol()
{
  // parse '{' new scope
  std::shared_ptr<token> token = peekToken();
  if (token->isTokenTypeSymbol() && token->getCharValue() == '{')
  {
    parseBody();
  }
  // parse ':' label
}

void parser::parseUnary()
{
  std::shared_ptr<token> operator_token = peekToken();
  if (STRINGS_EQUAL(operator_token->getStringValue().c_str(), "*"))
  {
    parseIndirectionUnary();
    return;
  }
  parseNormalUnary();
}

void parser::parseNormalUnary()
{
  std::shared_ptr<token> operator_token = nextToken();
  std::string operator_ = operator_token->getStringValue();
  parseExpression();
  std::shared_ptr<nodeExpression> operand_node = cast_node<nodeExpression>(popLastNode());
  std::shared_ptr<nodeExpression> unary_node = std::make_shared<nodeExpression>(NODE_TYPE_UNARY, operand_node->getFilePosition());
  unary_node->setValueNode(operand_node);
  unary_node->setStringValue(operator_token->getStringValue());
  pushNode(unary_node);
}

void parser::parseIndirectionUnary()
{
  std::shared_ptr<token> operator_token;
  std::shared_ptr<token> it_token;
  int pointer_depth = 0;
  do
  {
    pointer_depth++;
    operator_token = nextToken();
    it_token = peekToken();
  } while (STRINGS_EQUAL(it_token->getStringValue().c_str(), "*"));

  std::string operator_ = operator_token->getStringValue();
  parseExpression();
  std::shared_ptr<nodeExpression> operand_node = cast_node<nodeExpression>(popLastNode());
  std::shared_ptr<nodeExpression> unary_node = std::make_shared<nodeExpression>(NODE_TYPE_UNARY, operand_node->getFilePosition());
  unary_node->setValueNode(operand_node);
  unary_node->setStringValue(operator_);
  unary_node->setUnaryIndirectionDepth(pointer_depth);
  unary_node->setDatatype(operand_node->getDatatype());
  pushNode(unary_node);
}

std::shared_ptr<datatype> parser::parseDatatype()
{
  // to parse: static const long long*** name....
  std::shared_ptr<token> token = peekToken();
  std::shared_ptr<datatype> dtype = std::make_shared<datatype>(token->getFilePosition());

  // static const
  while (datatype::isKeywordVariableModifier(token->getStringValue()))
  {
    token = nextToken();
    dtype->setKeyVariableModifier(token->getStringValue());
    token = peekToken();
  }

  // long long
  while (datatype::IsKeywordDatatype(token->getStringValue()))
  {
    token = nextToken();
    dtype->setDataType(token->getStringValue());
    token = peekToken();
  }
  //***
  while (token->isTokenTypeOperator() && STRINGS_EQUAL(token->getStringValue().c_str(), "*"))
  {
    token = nextToken();
    dtype->incrementPointerDepth();
    token = peekToken();
  }

  return dtype;
}

void parser::_assert_(bool condition, std::string message)
{
  if (!condition)
  {
    if (!message.empty())
    {
      cerror(message.c_str());
    }
  }
  assert(condition);
};
