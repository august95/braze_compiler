#include "lexer.h"
#include "braze_compiler.h"


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
	/*
	token token = readNextToken();
	while (token)
	{
		tokens->push(token);
		token = readNextToken();
	}
	*/
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
