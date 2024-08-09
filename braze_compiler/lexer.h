#pragma once
//lexing source code to create tokens

class lexer
{
public:
	lexer();


private:

	char peekChar();
	char nextChar();
	void pushChar();

};

