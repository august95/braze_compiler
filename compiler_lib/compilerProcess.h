#pragma once

//container class for parser and lexer

#include "lexer.h"
#include "parser.h"
#include "codeGenerator.h"
#include <memory>


class compileProcess
{
public:
	compileProcess();
	void initialize(std::string filename);
	int startCompiler();

	//for unit tests
	std::list < std::shared_ptr < token > > getTokens() { return m_lexer.getTokens(); }
	std::list < std::shared_ptr < node > > getAbstractSyntaxTree() { return m_parser.getAbstractSyntaxTree(); }

	bool __unit_test_no_code_generation;
private:

	lexer m_lexer;
	parser m_parser;
	codeGenerator m_code_generator;

};

