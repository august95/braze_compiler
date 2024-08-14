#include "pch.h"

#include "../compiler_lib/compiler_lib.cpp"
#include "../compiler_lib/compilerProcess.h"
#include <string>
#include <list>

std::string file_path = "test_files/";

/*
TEST(lexer, template) {

	std::string file_name = "test_test.c";
	/*
	* File Content
	*
	* 01234 0x1af34 0b0101
	*/

/*

	const int num_of_tokens = 3;

	compileProcess process;
	process.intialize(file_path + file_name);
	process.startCompiler();

	std::list < std::shared_ptr < token > > tokens = process.getTokens();
	EXPECT_EQ(tokens.size(), num_of_tokens);

	std::shared_ptr < token > token = tokens.front();
	// perform test
	EXPECT_EQ();
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ();
	tokens.pop_front();
}

*/


TEST(lexer, symbols) {

	std::string file_name = "test_lexer_symbol.c";
	/*
	* File Content
	*
	* \ { } # )
	*/



	const int num_of_tokens = 5;

	compileProcess process;
	process.intialize(file_path + file_name);
	process.startCompiler();

	std::list < std::shared_ptr < token > > tokens = process.getTokens();
	EXPECT_EQ(tokens.size(), num_of_tokens);

	std::shared_ptr < token > token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), '\\');
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), '{');
	tokens.pop_front();


	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), '}');
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), '#');
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), ')');
	tokens.pop_front();
}



TEST(lexer, quote) {

	std::string file_name = "test_lexer_quote.c";
	/*
	* File Content
	*
	* 'a' 'x' '\n' '\\' 
	*/



	const int num_of_tokens = 4;

	compileProcess process;
	process.intialize(file_path + file_name);
	process.startCompiler();

	std::list < std::shared_ptr < token > > tokens = process.getTokens();
	EXPECT_EQ(tokens.size(), num_of_tokens);

	std::shared_ptr < token > token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), 'a');
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), 'x');
	tokens.pop_front();
	
	
	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), '\n');
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), '\\');
	tokens.pop_front();
}






TEST(lexer, stringsAndIncludeStrings) {

	std::string file_name = "test_lexer_string_and_include_strings.c";
	/*
	* File Content
	*
	* "braze" include <test_file.c> include "test.c" "test_string"
	*/



	const int num_of_tokens = 6;

	compileProcess process;
	process.intialize(file_path + file_name);
	process.startCompiler();

	std::list < std::shared_ptr < token > > tokens = process.getTokens();
	EXPECT_EQ(tokens.size(), num_of_tokens);

	std::shared_ptr < token > token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "braze");
	EXPECT_TRUE(token->isTokenTypeString());
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "include");
	EXPECT_TRUE(token->isTokenTypeKeyword());
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "test_file.c");
	EXPECT_TRUE(token->isTokenTypeString());
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "include");
	EXPECT_TRUE(token->isTokenTypeKeyword());
	tokens.pop_front();

	
	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "test.c");
	EXPECT_TRUE(token->isTokenTypeString());
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "test_string");
	EXPECT_TRUE(token->isTokenTypeString());
	tokens.pop_front();
}



TEST(lexer, identifierOrKeyword) {

	std::string file_name = "test_lexer_identifiers_symbols.c";
	/*
	* File Content
	*
	* testClass  return void  avxname ___braze
	*/

		const int num_of_tokens = 5;

		compileProcess process;
		process.intialize(file_path + file_name);
		process.startCompiler();

		std::list < std::shared_ptr < token > > tokens = process.getTokens();
		EXPECT_EQ(tokens.size(), num_of_tokens);

		std::shared_ptr < token > token = tokens.front();
		// perform test
		EXPECT_EQ(token->getStringValue(), "testClass");
		EXPECT_TRUE(token->isTokenTypeIdentifier());
		tokens.pop_front();

		token = tokens.front();
		// perform test
		EXPECT_EQ(token->getStringValue(), "return");
		EXPECT_TRUE(token->isTokenTypeKeyword());
		tokens.pop_front();

		token = tokens.front();
		// perform test
		EXPECT_EQ(token->getStringValue(), "void");
		EXPECT_TRUE(token->isTokenTypeKeyword());
		tokens.pop_front();

		token = tokens.front();
		// perform test
		EXPECT_EQ(token->getStringValue(), "avxname");
		EXPECT_TRUE(token->isTokenTypeIdentifier());
		tokens.pop_front();

		token = tokens.front();
		// perform test
		EXPECT_EQ(token->getStringValue(), "___braze");
		EXPECT_TRUE(token->isTokenTypeIdentifier());
		tokens.pop_front();


	}

	



TEST(lexer, operators)
{

	std::string file_name = "test_lexer_operators.c";
	/*
	* File Content
	*
	* + -  +=  == ... ^
	*/



	const int num_of_tokens = 6;

	compileProcess process;
	process.intialize(file_path + file_name);
	process.startCompiler();

	std::list < std::shared_ptr < token > > tokens = process.getTokens();
	EXPECT_EQ(tokens.size(), num_of_tokens);

	std::shared_ptr < token > token = tokens.front();
	EXPECT_EQ(token->getStringValue(), "+");
	tokens.pop_front();

	token = tokens.front();
	EXPECT_EQ(token->getStringValue(), "-");
	tokens.pop_front();

	 token = tokens.front();
	EXPECT_EQ(token->getStringValue(), "+=");
	tokens.pop_front();

	token = tokens.front();
	EXPECT_EQ(token->getStringValue(), "==");
	tokens.pop_front();

	token = tokens.front();
	EXPECT_EQ(token->getStringValue(), "...");
	tokens.pop_front();

	token = tokens.front();
	EXPECT_EQ(token->getStringValue(), "^");
	tokens.pop_front();

}

	

TEST(lexer, numbers) {

  std::string file_name = "test_lexer_numbers.c";
  /*
  * File Content
  * 
  * 01234 0x1af34 0b0101
  */ 


  const int num_of_tokens = 3;

  compileProcess process;
  process.intialize(file_path + file_name);
  process.startCompiler();

  std::list < std::shared_ptr < token > > tokens = process.getTokens();
  EXPECT_EQ(tokens.size(), num_of_tokens);
  
  std::shared_ptr < token > token = tokens.front();
  EXPECT_EQ(token->getNumberValue(), 1234);
  tokens.pop_front();

  token = tokens.front();
  EXPECT_EQ(token->getNumberValue(), 0x1af34);
  tokens.pop_front();

  token = tokens.front();
  EXPECT_EQ(token->getNumberValue(), 0b0101);
  tokens.pop_front();

}
TEST(lexer, ordinaryCFile) {

	std::string file_name = "test_lexer_ordinary_c_file.c";
	/*
	* File Content
	*
	* #include <iostram>
	*
	* int main()
	* {
	* 	string a = "abc";
	* 	bool var = true;
	* 	if (!var)
	* 	{
	* 		return 0xf;
	* 	}
	* }
	*/



	const int num_of_tokens = 38;

	compileProcess process;
	process.intialize(file_path + file_name);
	process.startCompiler();

	std::list < std::shared_ptr < token > > tokens = process.getTokens();
	EXPECT_EQ(tokens.size(), num_of_tokens);

	std::shared_ptr < token > token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), '#');
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "include");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "iostream");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_TRUE(token->isTokenTypeNewLine());
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "int");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "main");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "(");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), ')');
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_TRUE(token->isTokenTypeNewLine());
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), '{');
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_TRUE(token->isTokenTypeNewLine());
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "string");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "a");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "=");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "abc");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), ';');
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_TRUE(token->isTokenTypeNewLine());
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "bool");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "var");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "=");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "true");
	tokens.pop_front();


	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), ';');
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_TRUE(token->isTokenTypeNewLine());
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "if");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "(");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "!");
	tokens.pop_front();


	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "var");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), ')');
	tokens.pop_front();


	token = tokens.front();
	// perform test
	EXPECT_TRUE(token->isTokenTypeNewLine());
	tokens.pop_front();


	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), '{');
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_TRUE(token->isTokenTypeNewLine());
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getStringValue(), "return");
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getNumberValue(), 0xf);
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), ';');
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_TRUE(token->isTokenTypeNewLine());
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), '}');
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_TRUE(token->isTokenTypeNewLine());
	tokens.pop_front();

	token = tokens.front();
	// perform test
	EXPECT_EQ(token->getCharValue(), '}');
	tokens.pop_front();


}

