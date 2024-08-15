#pragma once

//receives a list of tokens, creates an abstract syntax tree with nodes

#include "token.h"
#include "node.h"
#include <list>
#include <iostream>


class parser
{
public:
	parser();

	void setTokenList(std::list < std::shared_ptr < token > > tokens) { m_tokens = tokens; };
	void startParser();

private:
	std::shared_ptr < token > nextToken();
	std::shared_ptr < token > peekToken();

	void parseTokens();
	void parseNextToken();

	void parseExpression();
	int parseExpressionOperatorOrOperand();

private:
	std::list < std::shared_ptr < node > > m_nodes;
	std::list < std::shared_ptr < token > > m_tokens;
};

