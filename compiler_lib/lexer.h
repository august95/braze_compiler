#pragma once
//lexing source code to create tokens

#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "filePosition.h"
#include "token.h"

class lexer
{
public:
	lexer();
	~lexer();

//TODO: these functions and ifstream should be provided by an interface(made by template?) 
//the lexer should not own the file stream
	char peekChar();
	char nextChar();
	void pushChar(char ch);


	void initialize(std::string filename);
	void startLexer();
	void lexFile();
	void _assert_(bool condition, std::string message);
	std::list < std::shared_ptr < token > > getTokens() { return tokens; }

private:


	std::shared_ptr < token > readNextToken();

	std::shared_ptr < token > makeOperatorTokenOrIncludeString();
	std::string getOperatorString();
	bool isOperatorValid(std::string _operator_);

	std::shared_ptr < token > makeNumberToken();
	std::shared_ptr < token > makeHexicalNumberToken();
	std::shared_ptr < token > makeBinaryNumberToken();
	std::shared_ptr < token > makeDecimalNumberToken();
	bool isHexChar(char c);

	std::shared_ptr < token > handle_whitespace();



	std::string m_filename;
	std::ifstream m_file;
	filePosition m_file_position;

	//list of tokens that has been lexed. Will be handed over to the parser
	std::list < std::shared_ptr < token > > tokens;
};

