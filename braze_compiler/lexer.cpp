#include "lexer.h"
#include "braze_compiler.h"


lexer::lexer() :
m_filename_to_lex("test.c"),
m_file(m_filename_to_lex)
{

}

lexer::~lexer()
{
	m_file.close();
}

char lexer::peekChar()
{
	if(m_file)
		return m_file.peek();
	cerror("LEX error: could not open test.c");
	return 'x';
}

char lexer::nextChar()
{
	if (m_file)
		return m_file.get();
	cerror("LEX error: could not open test.c");
	return 'x';
}

void lexer::pushChar(char ch)
{
	if (m_file)
		m_file.putback(ch);
	cerror("LEX error: could not open test.c");
}
