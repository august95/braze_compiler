#include "pch.h"
#include "compilerProcess.h"

compileProcess::compileProcess()
{

}

void compileProcess::intialize(std::string filename)
{
	lexer.initialize(filename);
}

void compileProcess::startCompiler()
{
	lexer.startLexer();
}
