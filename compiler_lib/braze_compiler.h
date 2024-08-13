// add headers that you want to pre-compile here
#include "framework.h"
#include <iostream>
#include <assert.h>

//TODO: add contex for where the error message was called. PARSER, LEXER, filename, line, col
static void cerror(const char* error) { std::cout << "Compiler Error: " << error << "\n"; }
static void cwarning(const char* warning) { std::cout << "Compiler Warning: " << warning << "\n"; }
static void clog(const char* log) { std::cout << "Log: " << log << "\n"; }
void _assert_(bool condition, std::string message)
{
	if (!condition)
	{
		if (!message.empty())
		{
			cerror(message.c_str());
		}
	}
	assert(condition);
};
