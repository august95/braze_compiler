#include "pch.h"
#include "lexer.h"
#include "braze_compiler.h"
#include <iostream>
#include <assert.h>

#define STRINGS_EQUAL(str, str2) \
    (str && str2 && (strcmp(str, str2) == 0))

#define CASE_NUMERIC \
    case '0':        \
    case '1':        \
    case '2':        \
    case '3':        \
    case '4':        \
    case '5':        \
    case '6':        \
    case '7':        \
    case '8':        \
    case '9'

#define CASE_OPERATOR \
    case '+':                            \
    case '-':                            \
    case '*':                            \
    case '>':                            \
    case '<':                            \
    case '^':                            \
    case '%':                            \
    case '!':                            \
    case '=':                            \
    case '~':                            \
    case '|':                            \
    case '&':                            \
    case '(':                            \
    case '[':                            \
    case ',':                            \
    case '.':                            \
    case '?'

lexer::lexer() :
m_filename(),
m_file(),
m_file_position()
{

}

lexer::~lexer()
{
	m_file.close();
}


char lexer::peekChar()
{
	if (m_file.is_open())
	{
		return m_file.peek();
	}
	cerror("could not open test.c");
	return 'x';
}

char lexer::nextChar()
{
	if (m_file.is_open())
	{
		char c = m_file.get();
		m_file_position.incrementCol();
		if (c == '\n')
		{
			m_file_position.incrementLine();
		}
		return c;
	}

	cerror("could not open test.c");
	return 'x';
}

void lexer::pushChar(char ch)
{
	if (!m_file.is_open())
	{
		cerror("LEX error: could not open test.c");
		return;
	}
	m_file.putback(ch);
}



void lexer::initialize(std::string filename)
{
	m_filename = filename;
	m_file.open(m_filename);
	m_file_position.setFileName(m_filename);
}

void lexer::startLexer()
{
	if (!m_file.is_open())
	{
		cerror("failed to open file");
		return;
	}

	lexFile();
}

void lexer::lexFile()
{
	/*
	* TODO:
	* creat token class
	* choose a RAII method, should I use smart pointers to tokens?
	* How should I store the tokens? a vector might work.
	* A parser needs to access the tokens in the order they was created, queue or linked list might do the work
	* 
	*/

	std::shared_ptr< token > token = readNextToken();
	while (token)
	{
		tokens.push_back(token);
		token = readNextToken();
	}
}

std::shared_ptr< token > lexer::readNextToken()
{
	std::shared_ptr < token > token(0);

	char c = peekChar();

	/*
	* TODO
	* 
	* comments
	* new line
	* symbol
	* expression depth???
	* quote ''
	* 
	*/
	
	switch (c)
	{
	case EOF:
		break;

	CASE_NUMERIC:
		token = makeNumberToken();
		break;

	CASE_OPERATOR:
		token = makeOperatorTokenOrIncludeString(); // '<' can be operator or part of #inlcude <...h>
		break;

	case '"':
		token = makeStringToken();
		break;

	case ' ':
	case '\t':
	case '\r':
		token = handle_whitespace();
		break;

	default:
		token = makeIdentifierOrKeyword();
		break;
	}

	return token;
}

std::shared_ptr < token > lexer::makeIdentifierOrKeyword()
{
	//might parse a variable 'myvar' or a keyword return
	char c = peekChar();
	if (isalpha(c) || c == '_')
	{	
		std::string identifier_or_keyword;
		while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
		{
			identifier_or_keyword += nextChar();
			c = peekChar();
		}
		if (is_keyword(identifier_or_keyword))
		{
			return std::make_shared < token >(tokenType::TOKEN_TYPE_KEYWORD, m_file_position, identifier_or_keyword); //keyword
		}

		return std::make_shared < token >(tokenType::TOKEN_TYPE_IDENTIFIER, m_file_position, identifier_or_keyword); //identifier
	}
	_assert_(false, "error could not lex identifier or keyword");
}

bool lexer::is_keyword(std::string _keyword_)
{
	return STRINGS_EQUAL(_keyword_.c_str(), "unsigned") ||
		STRINGS_EQUAL(_keyword_.c_str(), "signed") ||
		STRINGS_EQUAL(_keyword_.c_str(), "char") ||
		STRINGS_EQUAL(_keyword_.c_str(), "short") ||
		STRINGS_EQUAL(_keyword_.c_str(), "int") ||
		STRINGS_EQUAL(_keyword_.c_str(), "long") ||
		STRINGS_EQUAL(_keyword_.c_str(), "double") ||
		STRINGS_EQUAL(_keyword_.c_str(), "void") ||
		STRINGS_EQUAL(_keyword_.c_str(), "struct") ||
		STRINGS_EQUAL(_keyword_.c_str(), "union") ||
		STRINGS_EQUAL(_keyword_.c_str(), "static") ||
		STRINGS_EQUAL(_keyword_.c_str(), "__ignore_typecheck") ||
		STRINGS_EQUAL(_keyword_.c_str(), "return") ||
		STRINGS_EQUAL(_keyword_.c_str(), "include") ||
		STRINGS_EQUAL(_keyword_.c_str(), "sizeof") ||
		STRINGS_EQUAL(_keyword_.c_str(), "if") ||
		STRINGS_EQUAL(_keyword_.c_str(), "else") ||
		STRINGS_EQUAL(_keyword_.c_str(), "while") ||
		STRINGS_EQUAL(_keyword_.c_str(), "for") ||
		STRINGS_EQUAL(_keyword_.c_str(), "do") ||
		STRINGS_EQUAL(_keyword_.c_str(), "break") ||
		STRINGS_EQUAL(_keyword_.c_str(), "continue") ||
		STRINGS_EQUAL(_keyword_.c_str(), "switch") ||
		STRINGS_EQUAL(_keyword_.c_str(), "case") ||
		STRINGS_EQUAL(_keyword_.c_str(), "default") ||
		STRINGS_EQUAL(_keyword_.c_str(), "goto") ||
		STRINGS_EQUAL(_keyword_.c_str(), "typedef") ||
		STRINGS_EQUAL(_keyword_.c_str(), "const") ||
		STRINGS_EQUAL(_keyword_.c_str(), "extern") ||
		STRINGS_EQUAL(_keyword_.c_str(), "restrict");
}

std::shared_ptr < token > lexer::makeOperatorTokenOrIncludeString()
{
	//testing if lexing #include <file_name.c>
	if (lastTokenIsInlcudeKeyword())
	{
		std::string include_string = createString('<', '>');
		return std::make_shared < token >(tokenType::TOKEN_TYPE_STRING, m_file_position, include_string);
	}


	std::string operator_string = getOperatorString();
	return std::make_shared < token >(tokenType::TOKEN_TYPE_OPERATOR, m_file_position, operator_string);
}

std::string lexer::getOperatorString()
{
	//operator might be a single char then something else '+' 5, or two chars "+="
	char first_op = nextChar(); //'+'
	std::string operator_string("");
	operator_string += first_op;
	_assert_(isOperatorValid(operator_string),("'%s' is not a valid operator", operator_string.c_str()));
	operator_string += peekChar(); //'=' or 5
	if (isOperatorValid(operator_string)) //'+='
	{
		nextChar();
		return operator_string;
	}
	else if (STRINGS_EQUAL(operator_string.c_str(), "..")) // operator "..." infinite arguments
	{
		nextChar();
		_assert_(peekChar() == '.', "expected '.' after '..' to complete operator '...'");
		operator_string += nextChar();
		return operator_string;
	}
	operator_string.clear();
	operator_string += first_op; //'+' 5
	return operator_string; 

}

bool lexer::isOperatorValid(std::string _operator_)
{
	return STRINGS_EQUAL(_operator_.c_str(), "+") ||
		STRINGS_EQUAL(_operator_.c_str(), "-") ||
		STRINGS_EQUAL(_operator_.c_str(), "*") ||
		STRINGS_EQUAL(_operator_.c_str(), "/") ||
		STRINGS_EQUAL(_operator_.c_str(), "!") ||
		STRINGS_EQUAL(_operator_.c_str(), "^") ||
		STRINGS_EQUAL(_operator_.c_str(), "+=") ||
		STRINGS_EQUAL(_operator_.c_str(), "-=") ||
		STRINGS_EQUAL(_operator_.c_str(), "*=") ||
		STRINGS_EQUAL(_operator_.c_str(), "/=") ||
		STRINGS_EQUAL(_operator_.c_str(), ">>") ||
		STRINGS_EQUAL(_operator_.c_str(), "<<") ||
		STRINGS_EQUAL(_operator_.c_str(), "<=") ||
		STRINGS_EQUAL(_operator_.c_str(), ">=") ||
		STRINGS_EQUAL(_operator_.c_str(), "<") ||
		STRINGS_EQUAL(_operator_.c_str(), ">") ||
		STRINGS_EQUAL(_operator_.c_str(), "||") ||
		STRINGS_EQUAL(_operator_.c_str(), "&&") ||
		STRINGS_EQUAL(_operator_.c_str(), "|") ||
		STRINGS_EQUAL(_operator_.c_str(), "&") ||
		STRINGS_EQUAL(_operator_.c_str(), "++") ||
		STRINGS_EQUAL(_operator_.c_str(), "--") ||
		STRINGS_EQUAL(_operator_.c_str(), "=") ||
		STRINGS_EQUAL(_operator_.c_str(), "!=") ||
		STRINGS_EQUAL(_operator_.c_str(), "==") ||
		STRINGS_EQUAL(_operator_.c_str(), "->") ||
		STRINGS_EQUAL(_operator_.c_str(), "(") ||
		STRINGS_EQUAL(_operator_.c_str(), "[") ||
		STRINGS_EQUAL(_operator_.c_str(), ",") ||
		STRINGS_EQUAL(_operator_.c_str(), ".") ||
		STRINGS_EQUAL(_operator_.c_str(), "...") || // FIXME: add support
		STRINGS_EQUAL(_operator_.c_str(), "~") ||
		STRINGS_EQUAL(_operator_.c_str(), "?") ||
		STRINGS_EQUAL(_operator_.c_str(), "%");
}

bool lexer::lastTokenIsInlcudeKeyword()
{
	if (tokens.size() > 0)
	{
		std::shared_ptr < token > last_token = tokens.back();

		if (last_token->isTokenTypeKeyword() && STRINGS_EQUAL(last_token->getStringValue().c_str(), "include"))
		{
			return true;
		}
	}
	return false;
}

std::shared_ptr < token > lexer::makeStringToken()
{
	std::string _string = createString('"', '"');
	return std::make_shared < token >(tokenType::TOKEN_TYPE_STRING, m_file_position, _string);
}

std::string lexer::createString(char start_char, char end_char)
{
	_assert_(start_char == nextChar(),"expected beginning of string");
	char c = nextChar();
	std::string string_to_return;
	for (; c != end_char && c != EOF; c = nextChar())
	{
		if (c == '\\')
		{
			// we need to handle an escape character.
			continue;
		}
		string_to_return += c;
	}

	return string_to_return;
}

std::shared_ptr < token > lexer::makeNumberToken()
{
	std::shared_ptr < token > token(0);
	if (peekChar() == '0')
	{
		char c = peekChar();
		nextChar();

		//hex number 0xFA3
		if (peekChar() == 'x') 
		{
			return makeHexicalNumberToken();
		}

		//binary number: 0b1010
		else if (peekChar() == 'b')
		{
			return makeBinaryNumberToken();
		}

		//decimal number 0123, put back the '0' token we popped from the stream
		pushChar(c);
	}
	return makeDecimalNumberToken();
}

std::shared_ptr < token > lexer::makeHexicalNumberToken()
{
	_assert_(nextChar() == 'x', "expected char 'x'");
	unsigned long number = 0;
	std::string number_str;
	while (isHexChar(peekChar()))
	{
		number_str += peekChar();
		nextChar();
	}
	number = strtol(number_str.c_str(), 0, 16);
	return std::make_shared < token >(tokenType::TOKEN_TYPE_NUMBER, m_file_position, number);
}

std::shared_ptr < token > lexer::makeBinaryNumberToken()
{
	_assert_(nextChar() == 'b', "expected char 'b'");
	unsigned long number = 0;
	std::string number_str;
	while (peekChar() == '0' || peekChar() == '1')
	{
		number_str += peekChar();
		nextChar();
	}
	number = strtol(number_str.c_str(), 0, 2);
	return std::make_shared < token >(tokenType::TOKEN_TYPE_NUMBER, m_file_position, number);
}

std::shared_ptr < token > lexer::makeDecimalNumberToken()
{
	unsigned long number = 0;
	std::string number_str;
	while (peekChar() >= '0' && peekChar() <= '9')
	{
		number_str += peekChar();
		nextChar();
	}
	number = atoll(number_str.c_str());
	return std::make_shared < token >(tokenType::TOKEN_TYPE_NUMBER, m_file_position, number);
}

bool lexer::isHexChar(char c)
{
	char c_lower = tolower(c);
    return (c_lower >= '0' && c_lower <= '9') || (c_lower >= 'a' && c_lower <= 'f');
}


std::shared_ptr < token > lexer::handle_whitespace()
{
	_assert_(peekChar() == ' ' || peekChar() == '\t' || peekChar() == '\r', "expected ' ' '\t' or '\r'");
	nextChar();
	return readNextToken();
}
