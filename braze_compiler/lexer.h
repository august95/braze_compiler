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

	//TODO: these functions and ifstream should be provided by an interface(made by template?) 
	//the lexer should not own the file stream
	char peekChar();
	char nextChar();
	void pushChar(char ch);

	std::string m_filename_to_lex;
	std::ifstream m_file;
	
};

