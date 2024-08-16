#include "pch.h"
#include "token.h"

token::token()
	:m_token_tyoe(TOKEN_TYPE_NONE),
	m_file_position(),
	m_number_value(0),
	m_string_value(""),
	m_char_value(0)
{

}

token::token(tokenType token_type, filePosition file_position, unsigned long number_value)
	: m_token_tyoe(token_type),
	m_file_position(file_position),
	m_number_value(number_value),
	m_string_value(""),
	m_char_value(static_cast<char>(number_value))
{

}

token::token(tokenType token_type, filePosition file_position, std::string string_value)
	: m_token_tyoe(token_type),
	m_file_position(file_position),
	m_number_value(0),
	m_string_value(string_value),
	m_char_value(0)
{

}
