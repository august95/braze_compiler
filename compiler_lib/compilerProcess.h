#pragma once

//container class for parser and lexer

#include "lexer.h"
#include "parser.h"


class compileProcess
{
public:
	compileProcess();
	void initialize(std::string filename);
	void startCompiler();

	//for unit tests
	std::list < std::shared_ptr < token > > getTokens() { return lexer.getTokens(); }
	std::list < std::shared_ptr < node > > getAbstractSyntaxTree() { return parser.getAbstractSyntaxTree(); }
private:

	lexer lexer;
	parser parser;

};

