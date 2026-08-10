#include "../../source/pch.h"
#include "../preprocessorExpressionableParser.h"
#include "../preprocessor.h"

preProcessorExpressionableParser::preProcessorExpressionableParser(class preProcessor* preprocessor)
  :m_preprocessor(preprocessor)
{

}


void preProcessorExpressionableParser::parseUnary(std::shared_ptr< preProcessorExpressionable> expressionable)
{

}

void preProcessorExpressionableParser::parseExpression(std::shared_ptr< preProcessorExpressionable> expressionable)
{
  std::shared_ptr<nodePreProcessor> left_node = expressionable->peekLastNode();
  std::shared_ptr<token> operator_token = expressionable->peekToken(); 

  if (!left_node)
  {
    if (operator_token->isUnaryOperator())
    {
      parseUnary(expressionable);
    }
    else
    {
      cwarning("expression has no left operand, expected unary, but no found", operator_token->getFilePosition());
    }
    return;
  }
  expressionable->nextToken();
  expressionable->popLastNode();

  std::shared_ptr<token> peek_token = expressionable->peekToken();
  if (peek_token->isTokenTypeOperator())
  {
    if (STRINGS_EQUAL(peek_token->getStringValue().c_str(), "("))
    {
      //parse parentheses
    }
    if (peek_token->isUnaryOperator())
    {
      //parse unary
    }
    else
    {
      assert(0);
    }
  }
  else
  {
    parseTokens(expressionable);
  }
  std::shared_ptr<nodePreProcessor> right_node = expressionable->popLastNode(); // + L(30) R(20)
  std::shared_ptr<nodePreProcessor> expression_node = std::make_shared<nodePreProcessor>( NODE_TYPE_PREPROCESSOR_EXPRESSION, operator_token->getFilePosition());
  expression_node->setStringValue(operator_token->getStringValue());
  expression_node->setLeftNode(left_node);
  expression_node->setRightNode(right_node);
  precedenceHandler::reorderExpression<nodePreProcessor, NODE_TYPE_PREPROCESSOR_EXPRESSION>(expression_node);
  expression_node->setPreProcessor(m_preprocessor);

  expressionable->pushNode(expression_node);
}

void preProcessorExpressionableParser::parseOperator(std::shared_ptr< preProcessorExpressionable> expressionable)
{
  std::shared_ptr<token> token = expressionable->peekToken();
  if (STRINGS_EQUAL(token->getStringValue().c_str(), "("))
  {
    //parse parentheses
  }
  else if (STRINGS_EQUAL(token->getStringValue().c_str(), "?"))
  {
    //parse tenary
  }
  else
  {
    parseExpression(expressionable);
  }
}

bool preProcessorExpressionableParser::parseOperand(std::shared_ptr< preProcessorExpressionable> expressionable)
{
  bool continue_ = false;
  std::shared_ptr<token> token = expressionable->nextToken();
  std::shared_ptr<nodePreProcessor> node_;
  if (token->isTokenTypeNumber())
  {
    node_ = std::make_shared<nodePreProcessor>( NODE_TYPE_PREPROCESSOR_NUMBER, token->getFilePosition());
    node_->setPreProcessor(m_preprocessor);
    node_->setNumberValue(token->getNumberValue());
    continue_ = true;
    expressionable->pushNode(node_);
  }
  else if (token->isTokenTypeIdentifier())
  {
    if (STRINGS_EQUAL(token->getStringValue().c_str(), "defined"))
    {
      node_ = std::make_shared<nodePreProcessor>( NODE_TYPE_PREPROCESSOR_KEYWORD, token->getFilePosition());
      node_->setPreProcessor(m_preprocessor);
    }
    else
    {
      node_ = std::make_shared<nodePreProcessor>( NODE_TYPE_PREPROCESSOR_IDENTIFIER, token->getFilePosition());
      node_->setPreProcessor(m_preprocessor);
    }
    node_->setStringValue(token->getStringValue());

    continue_ = true;
    expressionable->pushNode(node_);
  }
  else if (token->isTokenTypeOperator())
  {
    parseOperator(expressionable);
  }
  return continue_;
}

bool preProcessorExpressionableParser::parseExpressionOperatorOrOperand(std::shared_ptr< preProcessorExpressionable> expressionable)
{
  std::shared_ptr<token> token = expressionable->peekToken();
  bool continue_ = false;
  if (!token)
  {
    return continue_;
  }
  if (token->isTokenTypeNumber() || token->isTokenTypeIdentifier())
  {
    continue_ = parseOperand(expressionable);
  }
  else if (token->isTokenTypeOperator())
  {
    parseOperator(expressionable);
  }
  return continue_;
}

bool preProcessorExpressionableParser::parseNextToken(std::shared_ptr< preProcessorExpressionable> expressionable)
{
  bool continue_ = false;
  std::shared_ptr<token> token = expressionable->peekToken();
  if (!token)
  {
    return continue_;
  }
  continue_ = parseExpressionOperatorOrOperand(expressionable);

  std::shared_ptr < nodePreProcessor > node_ = expressionable->popLastNode();
  if (node_->getNodeType() == NODE_TYPE_PREPROCESSOR_KEYWORD && STRINGS_EQUAL(node_->getStringValue().c_str(), "defined"))
  {
    //handle defined
  }
  expressionable->pushNode(node_);
  return continue_;
}

void preProcessorExpressionableParser::parseTokens(std::shared_ptr< preProcessorExpressionable> expressionable)
{
  while (parseNextToken(expressionable))
  {

  }
}


