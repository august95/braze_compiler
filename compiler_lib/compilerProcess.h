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

private:

	lexer lexer;
	parser parser;

};

