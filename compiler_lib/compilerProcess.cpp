#include "pch.h"
#include "compilerProcess.h"

compileProcess::compileProcess()
{

}

void compileProcess::initialize(std::string filename)
{
	lexer.initialize(filename);
}

void compileProcess::startCompiler()
{
	lexer.startLexer();
	parser.setTokenList(lexer.getTokens());
	parser.startParser();
}
