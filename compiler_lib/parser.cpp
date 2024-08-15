#include "pch.h"
#include "parser.h"


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
	return m_tokens.front();
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
	//while (parseExpressionOperatorOrOperand())
	{

	}
}

int parser::parseExpressionOperatorOrOperand()
{
	return 0;
}

