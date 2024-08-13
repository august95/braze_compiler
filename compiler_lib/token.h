#pragma once
#include "filePosition.h"


enum tokenType
{
	TOKEN_TYPE_NONE,
	TOKEN_TYPE_IDENTIFIER,
	TOKEN_TYPE_KEYWORD,
	TOKEN_TYPE_OPERATOR,
	TOKEN_TYPE_SYMBOL,
	TOKEN_TYPE_NUMBER,
	TOKEN_TYPE_STRING,
	TOKEN_TYPE_COMMENT,
	TOKEN_TYPE_NEWLINE
};

class token
{
public:
	token();
	token(tokenType token_type, filePosition file_position, unsigned int value);
	token(tokenType token_type, filePosition file_position, std::string string_value);


	unsigned long getNumberValue() { return m_number_value; }
	std::string getStringValue() { return m_string_value; }

private:
	tokenType m_token_tyoe;
	filePosition m_file_position;
	
	std::string m_string_value;
	unsigned long m_number_value;



public:
	bool isTokenTypeNone() { return m_token_tyoe == tokenType::TOKEN_TYPE_NONE; }
	bool isTokenTypeIdentifier() { return m_token_tyoe == tokenType::TOKEN_TYPE_IDENTIFIER; }
	bool isTokenTypeKeyword() { return m_token_tyoe == tokenType::TOKEN_TYPE_KEYWORD; }
	bool isTokenTypeOperator() { return m_token_tyoe == tokenType::TOKEN_TYPE_OPERATOR; }
	bool isTokenTypeSymbol() { return m_token_tyoe == tokenType::TOKEN_TYPE_SYMBOL; }
	bool isTokenTypeNumber() { return m_token_tyoe == tokenType::TOKEN_TYPE_NUMBER; }
	bool isTokenTypeString() { return m_token_tyoe == tokenType::TOKEN_TYPE_STRING; }
	bool isTokenTypeComment() { return m_token_tyoe == tokenType::TOKEN_TYPE_COMMENT; }
	bool isTokenTypeNewLine() { return m_token_tyoe == tokenType::TOKEN_TYPE_NEWLINE; }
};

