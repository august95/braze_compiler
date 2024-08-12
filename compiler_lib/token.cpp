#include "pch.h"
#include "token.h"

token::token()
	:m_token_tyoe(TOKEN_TYPE_NONE),
	m_number_value(0)
{

}

token::token(tokenType token_type, filePosition file_position, unsigned int number_value)
	: m_token_tyoe(token_type),
	m_file_position(file_position),
	m_number_value(number_value),
	m_string_value("")
{

}

token::token(tokenType token_type, filePosition file_position, std::string string_value)
	: m_token_tyoe(token_type),
	m_file_position(file_position),
	m_number_value(0),
	m_string_value(string_value)
{

}

bool token::isTokenTypeNone()
{
	return m_token_tyoe == tokenType::TOKEN_TYPE_NONE;
}
