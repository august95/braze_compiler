//global header file
#include <iostream>

static void cerror(const char* error) { std::cout << "Compiler Error: " << error << "\n"; }
static void cwarning(const char* warning) { std::cout << "Compiler Warning: " << warning << "\n"; }
static void clog(const char* log) { std::cout << "Log: " << log << "\n"; }

