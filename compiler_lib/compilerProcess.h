#pragma once

//container class for parser and lexer

#include "lexer.h"
#include "parser.h"
#include "codeGenerator.h"


class compileProcess
{
public:
	compileProcess();
	void initialize(std::string filename);
	int startCompiler();

	//for unit tests
	std::list < std::shared_ptr < token > > getTokens() { return lexer.getTokens(); }
	std::list < std::shared_ptr < node > > getAbstractSyntaxTree() { return parser.getAbstractSyntaxTree(); }

	bool __unit_test_no_code_generation;
private:

	lexer lexer;
	parser parser;
	codeGenerator code_generator;

};

