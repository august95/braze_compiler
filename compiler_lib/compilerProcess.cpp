#include "pch.h"
#include "compilerProcess.h"

compileProcess::compileProcess()
{

}

void compileProcess::intialize()
{
	lexer.initialize("test.c");
}

void compileProcess::startCompiler()
{
	lexer.startLexer();
}
