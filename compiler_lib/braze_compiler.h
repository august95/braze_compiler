// add headers that you want to pre-compile here
#include "framework.h"
#include <iostream>
#include <assert.h>

#include "filePosition.h"
#include "token.h"

//TODO: add contex for where the error message was called. PARSER, LEXER, filename, line, col
static void cerror(const char* error) 
{ 
	std::cout << "Compiler Error: " << error << "\n"; 
}

static void cwarning(const char* warning) 
{ 
	std::cout << "Compiler Warning: " << warning << "\n"; 
}

static void clog(const char* log) 
{ 
	std::cout << "Log: " << log << "\n"; 
}

static void cerror(const char* error, filePosition file_position)
{ 
	std::cout << "Compiler Error: " << error << "  in " << file_position.getLocationString().c_str() << "\n";
}

static void cwarning(const char* warning, filePosition file_position)
{ 
	std::cout << "Compiler Warning: " << warning << "  in " << file_position.getLocationString().c_str() << "\n";
}

static void clog(const char* log, filePosition file_position)
{ 
	std::cout << "Log: " << log << "  in " << file_position.getLocationString().c_str() << "\n";
}

