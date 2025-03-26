#include "pch.h"
#include "compilerProcess.h"
#include "braze_compiler.h"
#include <cstdio>  // For fopen, fclose, fread, fwrite, etc.


compileProcess::compileProcess()
{

}
void compileProcess::initialize(std::string filename)
{
	lexer.initialize(filename);
	code_generator.setFileName(filename);
}

void compileProcess::startCompiler()
{
	int ret = lexer.startLexer();
	if (ret != 0)
	{
		cerror("failed to lex file!");
	}

	parser.setTokenList(lexer.getTokens());
	ret = parser.startParser();
	if (ret != 0)
	{
		cerror("failed to parse tokens into abstract syntax tree!");
	}

	code_generator.setAbstractSyntaxTree(parser.getAbstractSyntaxTree());
	ret = code_generator.startCodeGeneration();
	if (ret != 0)
	{
		cerror("failed to generate code from abstract syntax tree!");
	}

	//TODO: invoke nasm assembler with obj file containing assembly as parameter
}
