#pragma once
//lexing source code to create tokens

#include <iostream>
#include <fstream>
#include <string>
#include "filePosition.h"

class lexer
{
public:
	lexer();
	~lexer();

	void initialize(std::string filename);
	void startLexer();
	void lexFile();


private:

	//TODO: these functions and ifstream should be provided by an interface(made by template?) 
	//the lexer should not own the file stream
	char peekChar();
	char nextChar();
	void pushChar(char ch);

	std::string m_filename;
	std::ifstream m_file;
	filePosition m_file_position;
};

