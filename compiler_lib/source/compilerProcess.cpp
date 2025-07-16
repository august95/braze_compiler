#include "pch.h"
#include "../compilerProcess.h"
#include "../braze_compiler.h"
#include <cstdio>  // For fopen, fclose, fread, fwrite, etc.


compileProcess::compileProcess()
	:__unit_test_no_code_generation(false)
{

}
void compileProcess::initialize(std::string filename)
{
	m_lexer.initialize(filename);
	m_code_generator.setFileName(filename);
}

int compileProcess::startCompiler()
{
	int ret = m_lexer.startLexer();
	if (ret != 0)
	{
		cerror("failed to lex file!");
		return ret;
	}

	m_parser.setTokenList(m_lexer.getTokens());
	ret = m_parser.startParser();
	if (ret != 0)
	{
		cerror("failed to parse tokens into abstract syntax tree!");
		return ret;
	}

	if (__unit_test_no_code_generation) return 0;

	m_code_generator.setAbstractSyntaxTree(m_parser.getAbstractSyntaxTree());
	ret = m_code_generator.startCodeGeneration();
	if (ret != 0)
	{
		cerror("failed to generate code from abstract syntax tree!");
		return ret;
	}
	return ret;

	//TODO: invoke nasm assembler with obj file containing assembly as parameter
}

void compileProcess::stop()
{
	m_code_generator.generateLableCount(true);
}
