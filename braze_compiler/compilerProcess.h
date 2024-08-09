#pragma once

//container class for parser and lexer

#include "lexer.h"
#include"parser.h"


class compileProcess
{
public:
	compileProcess();


private:

	lexer lexer;
	parser parser;

};

