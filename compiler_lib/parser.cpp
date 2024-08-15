#include "pch.h"
#include "parser.h"


parser::parser()
{

}

void parser::startParser()
{
	std::shared_ptr < token > token = nextToken();
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
