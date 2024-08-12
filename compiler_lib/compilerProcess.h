#pragma once

//container class for parser and lexer

#include "lexer.h"
#include "parser.h"


class compileProcess
{
public:
	compileProcess();
	void intialize(std::string filename);
	void startCompiler();

	//for unit tests
	std::list < std::shared_ptr < token > > getTokens() { return lexer.getTokens(); }

private:

	lexer lexer;
	parser parser;

};

