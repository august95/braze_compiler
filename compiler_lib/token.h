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
	bool isTokenTypeNone();

private:
	tokenType m_token_tyoe;
	filePosition m_file_position;
	
	unsigned long m_value;

};

