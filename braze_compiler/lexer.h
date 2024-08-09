#pragma once
//lexing source code to create tokens

#include <iostream>
#include <fstream>
#include <string>

class lexer
{
public:
	lexer();
	~lexer();


private:

	char peekChar();
	char nextChar();
	void pushChar(char ch);

	std::string m_filename_to_lex;
	std::ifstream m_file;
	
};

