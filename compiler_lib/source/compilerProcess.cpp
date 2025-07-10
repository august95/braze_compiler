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
	lexer.initialize(filename);
	code_generator.setFileName(filename);
}

int compileProcess::startCompiler()
{
	int ret = lexer.startLexer();
	if (ret != 0)
	{
		cerror("failed to lex file!");
		return ret;
	}

	parser.setTokenList(lexer.getTokens());
	ret = parser.startParser();
	if (ret != 0)
	{
		cerror("failed to parse tokens into abstract syntax tree!");
		return ret;
	}

	if (__unit_test_no_code_generation) return 0;

	code_generator.setAbstractSyntaxTree(parser.getAbstractSyntaxTree());
	ret = code_generator.startCodeGeneration();
	if (ret != 0)
	{
		cerror("failed to generate code from abstract syntax tree!");
		return ret;
	}
	return ret;

	//TODO: invoke nasm assembler with obj file containing assembly as parameter
}
