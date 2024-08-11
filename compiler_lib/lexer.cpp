#include "pch.h"
#include "lexer.h"
#include "braze_compiler.h"
#include <iostream>
#include <assert.h>

#define CASE_NUMERIC \
    case '0':        \
    case '1':        \
    case '2':        \
    case '3':        \
    case '4':        \
    case '5':        \
    case '6':        \
    case '7':        \
    case '8':        \
    case '9'

lexer::lexer() :
m_filename(),
m_file(),
m_file_position()
{

}

lexer::~lexer()
{
	m_file.close();
}


char lexer::peekChar()
{
	if (m_file)
	{
		return m_file.peek();
	}
	cerror("could not open test.c");
	return 'x';
}

char lexer::nextChar()
{
	if (m_file)
	{
		char c = m_file.get();
		m_file_position.incrementCol();
		if (c == '\n')
		{
			m_file_position.incrementLine();
		}
		return c;
	}

	cerror("could not open test.c");
	return 'x';
}

void lexer::pushChar(char ch)
{
	if (!m_file)
		return;
	m_file.putback(ch);
	cerror("LEX error: could not open test.c");
}



void lexer::initialize(std::string filename)
{
	m_filename = filename;
	m_file.open(m_filename);
	m_file_position.setFileName(m_filename);
}

void lexer::startLexer()
{
	if (!m_file.is_open())
	{
		cerror("failed to open file");
		return;
	}

	lexFile();
}

void lexer::lexFile()
{
	/*
	* TODO:
	* creat token class
	* choose a RAII method, should I use smart pointers to tokens?
	* How should I store the tokens? a vector might work.
	* A parser needs to access the tokens in the order they was created, queue or linked list might do the work
	* 
	*/

	std::shared_ptr< token > token = readNextToken();
	while (token)
	{
		tokens.push_back(token);
		token = readNextToken();
	}

	std::cout << "test";
}


void lexer::_assert_(bool condition)
{
	assert(condition);
}

std::shared_ptr< token > lexer::readNextToken()
{
	std::shared_ptr < token > token(0);

	char c = peekChar();
	
	switch (c)
	{
	CASE_NUMERIC:
		token = makeNumberToken();
		break;
	}

	return token;
}

std::shared_ptr < token > lexer::makeNumberToken()
{
	std::shared_ptr < token > token(0);
	if (peekChar() == '0')
	{
		char c = peekChar();
		nextChar();

		//hex number 0xFA3
		if (peekChar() == 'x') 
		{
			return makeHexicalNumberToken();
		}

		//binary number: 0b1010
		else if (peekChar() == 'b')
		{
			return makeBinaryNumberToken();
		}

		//decimal number 0123, put back the '0' token we popped from the stream
		pushChar(c);
	}
	return makeDecimalNumberToken();
}

std::shared_ptr < token > lexer::makeHexicalNumberToken()
{
	_assert_(nextChar() == 'x');
	unsigned long number = 0;
	std::string number_str;
	while (isHexChar(peekChar()))
	{
		number_str += peekChar();
		nextChar();
	}
	number = strtol(number_str.c_str(), 0, 16);
	return std::make_shared < token >(tokenType::TOKEN_TYPE_NUMBER, m_file_position, number);
}

std::shared_ptr < token > lexer::makeBinaryNumberToken()
{
	_assert_(nextChar() == 'b');
	unsigned long number = 0;
	std::string number_str;
	while (peekChar() == '0' || peekChar() <= '1')
	{
		number_str += peekChar();
		nextChar();
	}
	number = strtol(number_str.c_str(), 0, 2);
	return std::make_shared < token >(tokenType::TOKEN_TYPE_NUMBER, m_file_position, number);
}

std::shared_ptr < token > lexer::makeDecimalNumberToken()
{
	unsigned long number = 0;
	std::string number_str;
	while (peekChar() >= '0' && peekChar() <= '9')
	{
		number_str += peekChar();
		nextChar();
	}
	number = atoll(number_str.c_str());
	return std::make_shared < token >(tokenType::TOKEN_TYPE_NUMBER, m_file_position, number);
}

bool lexer::isHexChar(char c)
{
	char c_lower = tolower(c);
    return (c_lower >= '0' && c_lower <= '9') || (c_lower >= 'a' && c_lower <= 'f');
}

