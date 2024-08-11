#include "pch.h"
#include "token.h"

token::token()
	:m_token_tyoe(TOKEN_TYPE_NONE),
	m_value(0)
{

}

token::token(tokenType token_type, filePosition file_position, unsigned int value)
	: m_token_tyoe(token_type),
	m_file_position(file_position),
	m_value(value)
{

}

bool token::isTokenTypeNone()
{
	return m_token_tyoe == tokenType::TOKEN_TYPE_NONE;
}
