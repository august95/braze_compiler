#include "pch.h"
#include "parser.h"
#include "braze_compiler.h"
#include "nodeExpresssion.h"
#include <memory>



parser::parser()
{

}

void parser::startParser()
{
	parseTokens();
}

std::shared_ptr < token > parser::nextToken()
{
	std::shared_ptr < token > token = m_tokens.front();
	m_tokens.pop_front();
	return token;
}

std::shared_ptr < token > parser::peekToken()
{
	if (m_tokens.empty())
		return std::make_shared < token >();
	return m_tokens.front();
}

void parser::pushNode(std::shared_ptr < node > node)
{
	m_nodes.push_back(node);
}

std::shared_ptr < node > parser::peekLastNodeExpect(nodeType node_type)
{
	std::shared_ptr < node > node = peekLastNode();
	_assert_(node->getNodeType() & node_type, "expected other node type");
	return node;
}

std::shared_ptr < nodeExpresssion > parser::makeExpressionNode(filePosition file_position, std::string operator_, std::shared_ptr < node > left_node, std::shared_ptr < node > right_node)
{
	std::shared_ptr < nodeExpresssion > left_exp_node = cast_node<nodeExpresssion>(left_node);
	std::shared_ptr < nodeExpresssion > right_exp_node = cast_node<nodeExpresssion>(right_node);
	std::shared_ptr < nodeExpresssion > expression_node = std::make_shared<nodeExpresssion>(NODE_TYPE_EXPRESSION, file_position, left_exp_node, right_exp_node);
	expression_node->setStringValue(operator_);
	return expression_node;
}

template <class nodeType>
std::shared_ptr<nodeType> parser::cast_node(std::shared_ptr<node> node_)
{
	// Use std::static_pointer_cast<nodeType> to cast from base class to derived class
	if (!node_)
		return std::shared_ptr<nodeType>();
	std::shared_ptr<nodeType> cast_node_ = std::static_pointer_cast<nodeType>(node_);
	return cast_node_;
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
	std::shared_ptr < token > token = peekToken();
	if (token->isTokenTypeNumber() || token->isTokenTypeIdentifier() ||	token->isTokenTypeString())
	{
		parseExpression();
	}
}

void parser::parseExpression()
{
	//parse all the tokens in the expression until the ';'. Example :a = b + 5;
 	while (parseExpressionOperatorOrOperand() == 0)
	{

	}
}

int parser::parseExpressionOperatorOrOperand()
{
	std::shared_ptr < token > token = peekToken();
	if (!token)
	{
		return -1;
	}
	if (token->isTokenTypeNumber() || token->isTokenTypeIdentifier())
	{
		parseSingleTokenToExpresssionNode();
		return 0;
	}
	else if (token->isTokenTypeOperator())
	{
		parseOperand();
		return 0;
	}
	//TODO: add keyword in case of cast:  int a = (int)b;

	return 1;
}

void parser::parseSingleTokenToExpresssionNode()
{
	std::shared_ptr < token > t = nextToken();
	std::shared_ptr < node > n;

	if (t->isTokenTypeNumber())
	{
		n = std::make_shared<nodeExpresssion>(nodeType::NODE_TYPE_NUMBER, t->getFilePosition(), t->getNumberValue());
	}
	else if (t->isTokenTypeIdentifier())
	{
		n = std::make_shared<nodeExpresssion>(nodeType::NODE_TYPE_IDENTIFIER, t->getFilePosition(), t->getStringValue());
	}
	else if (t->isTokenTypeString())
	{
		n = std::make_shared<nodeExpresssion>(nodeType::NODE_TYPE_STRING, t->getFilePosition(), t->getStringValue());
	}	
	pushNode(n);	
}

void parser::parseOperand()
{
	parseNormalExpression();
}

void parser::parseNormalExpression()
{
	// 50 + 30 * 20 
	std::shared_ptr < node > left_node = peekLastNode(); //50  
	std::shared_ptr <token> operatort_token = peekToken(); // +
	//std::string operator_ = operatort_token->getStringValue();

	if (!left_node || !left_node->isValidExpressionType())
	{
		//TODO: might be unary: *a, &a or !a
		// 
		//FIXME: create debug messages that separates LEXING from PARSING and prints error location in file
		//cwarning("left node in expression %i is not allowed in expression", left_node->getNodeType());
		return;
	}
	nextToken(); // operator token popped '+'
	popLastNode();  // 50
	parseExpression(); // parse 30 * 20
	std::shared_ptr < node > right_node = popLastNode(); // * (20) (30)
	std::shared_ptr < nodeExpresssion > expression_node = makeExpressionNode(operatort_token->getFilePosition(), operatort_token->getStringValue(), left_node, right_node);
	
	expression_node->reorderExpression();

	pushNode(expression_node);
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
