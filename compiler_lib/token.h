#pragma once
#include "filePosition.h"

//A single entity. A collection of tokens will represents all the content inside a source file,
//but in a format that makes it easier for the parser.

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
	token(tokenType token_type, filePosition file_position, unsigned long value);
	token(tokenType token_type, filePosition file_position, std::string string_value);


	unsigned long getNumberValue() { return m_number_value; }
	std::string getStringValue() { return m_string_value; }
	char getCharValue() { return m_char_value; }
	filePosition getFilePosition() { return m_file_position; }

private:
	tokenType m_token_tyoe;
	filePosition m_file_position;
	
	//create enum for all the keywords/operators to avoid dealing with strings in the parser?
	std::string m_string_value;
	unsigned long m_number_value;
	char m_char_value;



public:

	//called by the parser
	bool isTokenTypeNone()			{ return m_token_tyoe == tokenType::TOKEN_TYPE_NONE; }
	bool isTokenTypeIdentifier()	{ return m_token_tyoe == tokenType::TOKEN_TYPE_IDENTIFIER; }
	bool isTokenTypeKeyword()		{ return m_token_tyoe == tokenType::TOKEN_TYPE_KEYWORD; }
	bool isTokenTypeOperator()		{ return m_token_tyoe == tokenType::TOKEN_TYPE_OPERATOR; }
	bool isTokenTypeSymbol()		{ return m_token_tyoe == tokenType::TOKEN_TYPE_SYMBOL; }
	bool isTokenTypeNumber()		{ return m_token_tyoe == tokenType::TOKEN_TYPE_NUMBER; }
	bool isTokenTypeString()		{ return m_token_tyoe == tokenType::TOKEN_TYPE_STRING; }
	bool isTokenTypeComment()		{ return m_token_tyoe == tokenType::TOKEN_TYPE_COMMENT; }
	bool isTokenTypeNewLine()		{ return m_token_tyoe == tokenType::TOKEN_TYPE_NEWLINE; }
};

