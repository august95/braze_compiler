#include "pch.h"

#include "../compiler_lib/source/compiler_lib.cpp"
#include "../compiler_lib/compilerProcess.h"
#include "../compiler_lib/scope.h"
#include "test_helper.cpp"
#include <string>
#include <list>
#include <iostream>
#include <sstream>

//__LOCAL__ is defined in unit_test.vcxproj preprocessor definitions, and is defined if the project was opened with the sln visual studio file.
//The github action requires another filepath to the test files. 
#ifdef __LOCAL__
std::string file_path = "test_files/";
#else //Github action
std::string file_path = "D:/a/braze_compiler/braze_compiler/unit_test/test_files/";
#endif // __LOCAL__



TEST(lexer, symbols) {

  std::string file_name = "lexer/test_lexer_symbol.c";
  //
  // File Content
  //
  //\ { } # )
  //



  const int num_of_tokens = 5;

  lexer lexer;
  lexer.initialize(file_path + file_name);
  lexer.startLexer();

  std::list < std::shared_ptr < token > > tokens = lexer.getTokens();
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

  std::string file_name = "lexer/test_lexer_quote.c";
  //
  // File Content
  //
  //'a' 'x' '\n' '\\' 
  //



  const int num_of_tokens = 4;

  lexer lexer;
  lexer.initialize(file_path + file_name);
  lexer.startLexer();

  std::list < std::shared_ptr < token > > tokens = lexer.getTokens();
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

  std::string file_name = "lexer/test_lexer_string_and_include_strings.c";
  //
  // File Content
  //
  //"braze" include <test_file.c> include "test.c" "test_string"
  //



  const int num_of_tokens = 6;

  lexer lexer;
  lexer.initialize(file_path + file_name);
  lexer.startLexer();

  std::list < std::shared_ptr < token > > tokens = lexer.getTokens();
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

  std::string file_name = "lexer/test_lexer_identifiers_symbols.c";
  //
  // File Content
  //
  //testClass  return void  avxname ___braze
  //

    const int num_of_tokens = 5;

    lexer lexer;
    lexer.initialize(file_path + file_name);
    lexer.startLexer();

    std::list < std::shared_ptr < token > > tokens = lexer.getTokens();
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

  std::string file_name = "lexer/test_lexer_operators.c";
  //
  // File Content
  //
  //+ -  +=  == ... ^
  //



  const int num_of_tokens = 6;

  lexer lexer;
  lexer.initialize(file_path + file_name);
  lexer.startLexer();

  std::list < std::shared_ptr < token > > tokens = lexer.getTokens();
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

  std::string file_name = "lexer/test_lexer_numbers.c";
  //
  // File Content
  //
  //01234 0x1af34 0b0101
  // 


  const int num_of_tokens = 3;

  lexer lexer;
  lexer.initialize(file_path + file_name);
  lexer.startLexer();

  std::list < std::shared_ptr < token > > tokens = lexer.getTokens();
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

  std::string file_name = "lexer/test_lexer_ordinary_c_file.c";
  //
  // File Content
  //
  //#include <iostram>
  //
  //int main()
  //{
  //  string a = "abc";
  //  bool var = true;
  //   if (!var)
  //  {
  //    return 0xf;
  //  }
  //}
  //



  const int num_of_tokens = 38;

  lexer lexer;
  lexer.initialize(file_path + file_name);
  lexer.startLexer();

  std::list < std::shared_ptr < token > > tokens = lexer.getTokens();
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




TEST(parser, string) {

  std::string file_name = "parser/test_parser_string.c";

  //"test string"
  const int num_of_tokens = 5;

  compileProcess process;
  process.__unit_test_no_code_generation = true;
  process.initialize(file_path + file_name);
  process.startCompiler();

  std::list < std::shared_ptr < node > > ast = process.getAbstractSyntaxTree();
  std::shared_ptr < node > node = ast.front();
  EXPECT_EQ( node->getStringValue(), "test string");
}


TEST(parser, expression) {

  std::string file_name = "parser/test_parser_test.c";
  //
  // File Content
  //
  // a = 50 * 30 + 20
  //
  const int num_of_tokens = 5;

  compileProcess process;
  process.__unit_test_no_code_generation = true;
  process.initialize(file_path + file_name);
  process.startCompiler();

  std::list < std::shared_ptr < node > > ast = process.getAbstractSyntaxTree();
  std::shared_ptr < node > node = ast.front();
  EXPECT_EQ(node->getStringValue(), "=");
  EXPECT_EQ(node->getLeftNode()->getStringValue(), "a");
  EXPECT_EQ(node->getRightNode()->getStringValue(), "+");
  EXPECT_EQ(node->getRightNode()->getRightNode()->getNumberValue(), 20);
  EXPECT_EQ(node->getRightNode()->getLeftNode()->getStringValue(), "*");
  EXPECT_EQ(node->getRightNode()->getLeftNode()->getRightNode()->getNumberValue(), 30);
  EXPECT_EQ(node->getRightNode()->getLeftNode()->getLeftNode()->getNumberValue(), 50);

}


TEST(parser, keyword) {

  std::string file_name = "parser/test_parser_keyword.c";
  //
  // File Content
  //
  //  static const long long*** var_name = 50 * 30 + 20;
  //
  const int num_of_tokens = 5;

  compileProcess process;
  process.__unit_test_no_code_generation = true;
  process.initialize(file_path + file_name);
  process.startCompiler();

  std::list < std::shared_ptr < node > > ast = process.getAbstractSyntaxTree();
  std::shared_ptr < node > _node = ast.front();  
  std::shared_ptr < datatype > dtype = _node->getDatatype();
  EXPECT_TRUE(dtype->isStatic());
  EXPECT_TRUE(dtype->isConst());
  EXPECT_TRUE(dtype->secondaryPrimitiveType());
  EXPECT_EQ(dtype->getPrimitiveType(), primitiveType::DATA_TYPE_LONG);
  EXPECT_EQ(dtype->getSecondPrimitiveType(), primitiveType::DATA_TYPE_LONG);
  EXPECT_EQ(dtype->getDatatypeSize(), 4);

  EXPECT_EQ(_node->getStringValue(), "var_name");

  std::shared_ptr < node > val_node = _node->getValueNode();
  EXPECT_EQ(val_node->getStringValue(), "+");
  EXPECT_EQ(val_node->getRightNode()->getNumberValue(), 20);
  EXPECT_EQ(val_node->getLeftNode()->getStringValue(), "*");
  EXPECT_EQ(val_node->getLeftNode()->getRightNode()->getNumberValue(), 30);
  EXPECT_EQ(val_node->getLeftNode()->getLeftNode()->getNumberValue(), 50);


}


TEST(parser, function) {

  std::string file_name = "parser/test_parser_function.c";


//  int main()
//  {
//  int var_val;
//  int var_b = 0;
//  var_val + 50;
//  }

  const int num_of_tokens = 5;

  compileProcess process;
  process.__unit_test_no_code_generation = true;
  process.initialize(file_path + file_name);
  process.startCompiler();

  std::list < std::shared_ptr < node > > ast = process.getAbstractSyntaxTree();
  std::shared_ptr < node > _node = ast.front();

  EXPECT_EQ(_node->getStringValue(), "main");
  EXPECT_EQ(_node->getBodyNode()->getBodySize(), 8 );
  std::list < std::shared_ptr < node > > statements = _node->getBodyNode()->getStatements();
  EXPECT_EQ(statements.size(), 3 );

  statements.pop_back(); // var_val + 50
  std::shared_ptr < node > var_a = statements.back();
  EXPECT_EQ(var_a->getDatatypeSize(), 4); //int var_b;
  EXPECT_EQ(var_a->getStackOffset(), -8);// int var_b;
  statements.pop_back();

  var_a = statements.back();
  EXPECT_EQ(var_a->getDatatypeSize(), 4); //int var_val;
  EXPECT_EQ(var_a->getStackOffset(), -4);// int var_val;

}


TEST(parser, ifstatement) {

  std::string file_name = "parser/test_parser_if_statement.c";


  //int main()
  //{
  //  if (1)
  //  {
  //    int a = 0;
  //  }
  //}


  const int num_of_tokens = 5;

  compileProcess process;
  process.__unit_test_no_code_generation = true;
  process.initialize(file_path + file_name);
  process.startCompiler();

  std::list < std::shared_ptr < node > > ast = process.getAbstractSyntaxTree();
  std::shared_ptr < node > _node = ast.front();

  EXPECT_EQ(_node->getStringValue(), "main");
  EXPECT_EQ(_node->getBodyNode()->getBodySize(), 4 );

  std::list < std::shared_ptr < node > > statements = _node->getBodyNode()->getStatements();
  EXPECT_EQ(statements.size(), 1 );

  std::shared_ptr < node > if_node = statements.back();

  EXPECT_TRUE(if_node->getNodeType() == NODE_TYPE_STATEMENT_IF);
  EXPECT_EQ(if_node->getBodyNode()->getBodySize(), 4);
  EXPECT_TRUE(if_node->getConditionNode()->getNodeType() == NODE_TYPE_NUMBER);
  EXPECT_TRUE(if_node->getConditionNode()->getNumberValue() == 1);
}




TEST(parser, whilestatement) {

  std::string file_name = "parser/test_parser_while_statement.c";

  //int main()
  //{
  //  int a = 5;
  //  if (a > 10)
  //  {
  //    int a = 10;
  //  }
  //  else if (a < 10)
  //  {
  //    int b = a;
  //  }
  //  else
  //  {
  //    int c = 3;
  //  }
  //}


  const int num_of_tokens = 5;

  compileProcess process;
  process.__unit_test_no_code_generation = true;
  process.initialize(file_path + file_name);
  process.startCompiler();

  std::list < std::shared_ptr < node > > ast = process.getAbstractSyntaxTree();
  std::shared_ptr < node > _node = ast.front();

  EXPECT_EQ(_node->getStringValue(), "main");
  EXPECT_EQ(_node->getBodyNode()->getBodySize(), 4);

  std::list < std::shared_ptr < node > > statements = _node->getBodyNode()->getStatements();
  EXPECT_EQ(statements.size(), 1);

  std::shared_ptr < node > while_node = statements.back();

  EXPECT_TRUE(while_node->getNodeType() == NODE_TYPE_STATEMENT_WHILE);
  EXPECT_EQ(while_node->getBodyNode()->getBodySize(), 4);
  EXPECT_TRUE(while_node->getConditionNode()->getNodeType() == NODE_TYPE_NUMBER);
  EXPECT_TRUE(while_node->getConditionNode()->getNumberValue() == 1);

  process.stop();
}

TEST(parser, forstatement) {

  std::string file_name = "parser/test_parser_for_statement.c";

  //int main()
  //{
  //  for (int i = 0; i < 10; i = i + 1)
  //  {
  //    int a = 5;
  //  }
  //}


  const int num_of_tokens = 5;

  compileProcess process;
  process.__unit_test_no_code_generation = true;
  process.initialize(file_path + file_name);
  process.startCompiler();

  std::list < std::shared_ptr < node > > ast = process.getAbstractSyntaxTree();
  std::shared_ptr < node > _node = ast.front();

  EXPECT_EQ(_node->getStringValue(), "main");
  EXPECT_EQ(_node->getBodyNode()->getBodySize(), 8);

  std::list < std::shared_ptr < node > > statements = _node->getBodyNode()->getStatements();
  EXPECT_EQ(statements.size(), 1);

  std::shared_ptr < node > for_node = statements.back();

  EXPECT_TRUE(for_node->getNodeType() == NODE_TYPE_STATEMENT_FOR);
  EXPECT_EQ(for_node->getBodyNode()->getBodySize(), 4);

  EXPECT_TRUE(for_node->getConditionNode()->getNodeType() == NODE_TYPE_EXPRESSION);
  EXPECT_TRUE(STRINGS_EQUAL(for_node->getConditionNode()->getStringValue().c_str(), "<"));


  EXPECT_TRUE(for_node->getInitNode()->getNodeType() == NODE_TYPE_VARIABLE);
  EXPECT_TRUE(STRINGS_EQUAL(for_node->getInitNode()->getStringValue().c_str(), "i"));

  EXPECT_TRUE(for_node->getLoopNode()->getNodeType() == NODE_TYPE_EXPRESSION);
  EXPECT_TRUE(STRINGS_EQUAL(for_node->getLoopNode()->getStringValue().c_str(), "="));



  process.stop();
}



TEST(parser, globalAccesFromFunction) {

  std::string file_name = "parser/test_parser_global_access_from_function.c";


  //  int main()
  //  {
  //  int var_val;
  //  int var_b = 0;
  //  var_val + 50;
  //  }

  const int num_of_tokens = 5;

  compileProcess process;
  process.__unit_test_no_code_generation = true;
  process.initialize(file_path + file_name);
  process.startCompiler();

  std::list < std::shared_ptr < node > > ast = process.getAbstractSyntaxTree();
  std::shared_ptr < node > global = ast.front();
  ast.pop_front();
  std::shared_ptr < node > _node = ast.front();

  EXPECT_EQ(_node->getStringValue(), "main");
  EXPECT_EQ(_node->getBodyNode()->getBodySize(), 8);
  std::list < std::shared_ptr < node > > statements = _node->getBodyNode()->getStatements();
  EXPECT_EQ(statements.size(), 3);

  statements.pop_back(); // var_val + 50
  std::shared_ptr < node > var_b = statements.back();
  EXPECT_EQ(var_b->getDatatypeSize(), 4); //int var_b;
  EXPECT_EQ(var_b->getStackOffset(), -8);// int var_b;
  //testing symbol resolver, needs to find the declaration node for identifiers
  EXPECT_TRUE(STRINGS_EQUAL(var_b->getValueNode()->getRightNode()->getDeclarationNode()->getStringValue().c_str(), global->getStringValue().c_str())); 
  statements.pop_back();

  var_b = statements.back();
  EXPECT_EQ(var_b->getDatatypeSize(), 4); //int var_val;
  EXPECT_EQ(var_b->getStackOffset(), -4);// int var_val;

}

TEST(parser, functionArgumets) {

  std::string file_name = "parser/test_parser_function_arguments.c";


  //  int main(int a , char b)
  //  {
  //  int var_val;
  //  int var_b = 0;
  //  var_val + 50;
  //  }

  const int num_of_tokens = 5;

  compileProcess process;
  process.__unit_test_no_code_generation = true;
  process.initialize(file_path + file_name);
  process.startCompiler();

  std::list < std::shared_ptr < node > > ast = process.getAbstractSyntaxTree();
  std::shared_ptr < node > _node = ast.front();

  EXPECT_EQ(_node->getStringValue(), "main");

  std::list < std::shared_ptr < node > > arguments = _node->getFunctionArguments();

  std::shared_ptr < node > a = arguments.back();
  EXPECT_TRUE(STRINGS_EQUAL(a->getStringValue().c_str(), "b")); //int var_b;
  EXPECT_EQ(a->getDatatypeSize(), 1); //int var_b;
  EXPECT_EQ(a->getStackOffset(), 12);// int var_b;
  arguments.pop_back();

  a = arguments.back();
  EXPECT_TRUE(STRINGS_EQUAL(a->getStringValue().c_str(), "a")); //int var_b;
  EXPECT_EQ(a->getDatatypeSize(), 4); //int var_b;
  EXPECT_EQ(a->getStackOffset(), 8);// int var_b;


  EXPECT_EQ(_node->getBodyNode()->getBodySize(), 8);
  std::list < std::shared_ptr < node > > statements = _node->getBodyNode()->getStatements();
  EXPECT_EQ(statements.size(), 3);

  statements.pop_back(); // var_val + 50
  std::shared_ptr < node > var_a = statements.back();
  EXPECT_EQ(var_a->getDatatypeSize(), 4); //int var_b;
  EXPECT_EQ(var_a->getStackOffset(), -8);// int var_b;
  statements.pop_back();

  var_a = statements.back();
  EXPECT_EQ(var_a->getDatatypeSize(), 4); //int var_val;
  EXPECT_EQ(var_a->getStackOffset(), -4);// int var_val;

}


TEST(parser, functionWithSecondScope) {

//
// File Content
// 
//  int main() 
//  {
//    int var_a;
//    {
//      int var_val;
//      var_val + 50;
//      int var_b;
//    }
//   int var_c;
//  }
//
  std::string file_name = "parser/test_parser_function_2.c";

  const int num_of_tokens = 5;

  compileProcess process;
  process.__unit_test_no_code_generation = true;
  process.initialize(file_path + file_name);
  process.startCompiler();

  std::list < std::shared_ptr < node > > ast = process.getAbstractSyntaxTree();
  std::shared_ptr < node > _node = ast.front();

  EXPECT_EQ(_node->getStringValue(), "main");
  EXPECT_EQ(_node->getBodyNode()->getBodySize(), 16);
  EXPECT_EQ(_node->getReturnDatatype()->getDatatypeSize(), 4);
  EXPECT_EQ(_node->getReturnDatatype()->getPrimitiveType(), primitiveType::DATA_TYPE_INTEGER);
  
  std::list < std::shared_ptr < node > > statements = _node->getBodyNode()->getStatements();
  EXPECT_EQ(statements.size(), 3);

  std::shared_ptr < node > var_c = statements.back();
  statements.pop_back();
  std::shared_ptr < node > nested_body_node = statements.back(); // { int var_val; var_val + 50; int var_b;  }
  
  statements.pop_back(); //statemnets.back() now becomes int var_a as the nested body noed is popped

  std::shared_ptr < node > var_a = statements.back();
  EXPECT_EQ(var_a->getDatatypeSize(),4); //int var_a;
  EXPECT_EQ(var_a->getStackOffset(), -4);// int var_a;

  EXPECT_EQ(nested_body_node->getBodySize(), 8);
  std::list < std::shared_ptr < node > > nested_statements = nested_body_node->getStatements();
  EXPECT_EQ(nested_statements.size(), 3);

  std::list < std::shared_ptr < node > > nested_statments = nested_body_node->getStatements();


  EXPECT_EQ(nested_statments.front()->getDatatypeSize(), 4); //int var_val;
  EXPECT_EQ(nested_statments.front()->getStackOffset(), -8);// int var_val;

  EXPECT_EQ(nested_statments.back()->getDatatypeSize(), 4); //int var_b;
  EXPECT_EQ(nested_statments.back()->getStackOffset(), -12);// int var_b;

  
  EXPECT_EQ(var_c->getDatatypeSize(), 4); //int var_c;
  EXPECT_EQ(var_c->getStackOffset(), -16);// int var_c;
}



TEST(codegen, globalVariables) {

//  int a;
//  int b = 5;
//  int c = 0xb;
//  char d = 'a';


  std::string target =
    "section .data\n"
//    "; int a\n"
    "a: dd 0\n"
//    "; int b\n"
    "b: dd 5\n"
//    "; int c\n"
    "c: dd 11\n"
//    "; char d\n"
    "d: db 97\n"
    "section .text\n"
    "section .rodata\n";

  std::string file_name = "codegeneration/test_codegen_global_variables.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();



  EXPECT_TRUE(compareFiles(target, file_path + asm_file));


}


TEST(parser, unary) {

  std::string file_name = "parser/test_parser_unary.c";

  //int main()
  //{
  //  int* ptr;
  //  int val = 5;
  //  ptr = &val;
  //
  //  int result = *ptr;
  //}

  const int num_of_tokens = 5;

  compileProcess process;
  process.__unit_test_no_code_generation = true;
  process.initialize(file_path + file_name);
  process.startCompiler();

  std::list < std::shared_ptr < node > > ast = process.getAbstractSyntaxTree();
  std::shared_ptr < node > _node = ast.front();


  EXPECT_EQ(_node->getStringValue(), "main");
  EXPECT_EQ(_node->getBodyNode()->getBodySize(), 12);
  std::list < std::shared_ptr < node > > statements = _node->getBodyNode()->getStatements();
  EXPECT_EQ(statements.size(), 4);

  std::shared_ptr < node > ptr = statements.front();
  EXPECT_EQ(ptr->getDatatypeSize(), 4);
  EXPECT_EQ(ptr->getStackOffset(), -4);
  EXPECT_EQ(ptr->getDatatype()->getPointerDepth(), 1);
  statements.pop_front();
  statements.pop_front();// int val

  std::shared_ptr < node > ptr_assignment = statements.front();
  EXPECT_EQ(ptr_assignment->getRightNode()->getNodeType(), nodeType::NODE_TYPE_UNARY);
  EXPECT_TRUE(STRINGS_EQUAL(ptr_assignment->getRightNode()->getStringValue().c_str(), "&"));

  statements.pop_front();// int val
  std::shared_ptr < node > result = statements.front();
  EXPECT_EQ(result->getValueNode()->getNodeType(), nodeType::NODE_TYPE_UNARY);
  EXPECT_TRUE(STRINGS_EQUAL(result->getValueNode()->getStringValue().c_str(), "*"));


}


TEST(codegen, function) {

//  int var = 0;
//
//  int main()
//  {
//    int a = 0;
//    int b = 5;
//    a = var;
//    var = b + a;
//  }


  std::string target =
    "section .data\n"
    //"; int var\n"
    "var: dd 0\n"
    "section .text\n"
    "global main\n"
    //"; main function\n"
    "main:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
    "push dword 0\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "push dword 5\n"
    "pop eax\n"
    "mov dword [ebp-8], eax\n"
    "push dword [var]\n"
    //local variables a and b has been generated, start on assignments a=var;
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "push dword [ebp-8]\n"
    "push dword [ebp-4]\n"
    "pop ecx\n"
    "pop eax\n"
    "add eax, ecx\n"
    "push eax\n"
    "pop eax\n"
    "mov dword [var], eax\n"
    //leave function
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "section .rodata\n";



  std::string file_name = "codegeneration/test_codegen_function.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
//  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));
  
}

TEST(codegen, functionArguments) {

  //
  //int var = 0;
  //
  //int main(bool c, int d)
  //{
  //  bool b = c;
  //  long a = d;
  //  int e = 5;
  //  var = d;
  //}


  std::string target =
    "section .data\n"
    //    "; int var\n"
    "var: dd 0\n"
    "section .text\n"
    "global main\n"
    //    "; main function\n"
    "main:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
    "mov eax, [ebp+8]\n"
    "movzx eax, al\n"
    "push eax\n"
    "pop eax\n"
    "mov byte [ebp-1], al\n"
    "push dword [ebp+12]\n"
    "pop eax\n"
    "mov dword [ebp-8], eax\n"
    "push dword 5\n"
    "pop eax\n"
    "mov dword [ebp-12], eax\n"
    "push dword [ebp+12]\n"
    "pop eax\n"
    "mov dword [var], eax\n"
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "section .rodata\n";




  std::string file_name = "codegeneration/test_codegen_function_arguments.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
  //  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));

}

TEST(codegen, functionArguments2) {

  //int var = 0;
  //
  //int main(char c, int d, char e , long f, int g, int h)
  //{
  //  char l = c;
  //  long m = d;
  //  int r = h;
  //  int n = 5;
  //  long o = f;
  //  char p = e;
  //  int w = g;
  //  var = d;
  //}


  std::string target =
    "section .data\n"
//    "; int var\n"
    "var: dd 0\n"
    "section .text\n"
    "global main\n"
//    "; main function\n"
    "main:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 32\n"
    "mov eax, [ebp+8]\n"
    "movzx eax, al\n"
    "push eax\n"
    "pop eax\n"
    "mov byte [ebp-1], al\n"
    "push dword [ebp+12]\n"
    "pop eax\n"
    "mov dword [ebp-8], eax\n"
    "push dword [ebp+28]\n"
    "pop eax\n"
    "mov dword [ebp-12], eax\n"
    "push dword 5\n"
    "pop eax\n"
    "mov dword [ebp-16], eax\n"
    "push dword [ebp+20]\n"
    "pop eax\n"
    "mov dword [ebp-20], eax\n"
    "mov eax, [ebp+16]\n"
    "movzx eax, al\n"
    "push eax\n"
    "pop eax\n"
    "mov byte [ebp-21], al\n"
    "push dword [ebp+24]\n"
    "pop eax\n"
    "mov dword [ebp-28], eax\n"
    "push dword [ebp+12]\n"
    "pop eax\n"
    "mov dword [var], eax\n"
    "add esp, 32\n"
    "pop ebp\n"
    "ret\n"
    "section .rodata\n";




  std::string file_name = "codegeneration/test_codegen_function_arguments2.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
  //  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));

}

TEST(codegen, functionArguments3) {


  //  int var = 0;
  //
  //  int main(char c, int d, char x, long y)
  //  {
  //    char b = c;
  //    long a = d;
  //    int e = 5;
  //    d = e;
  //    x = 'a';
  //    var = d;
  //    y = 10;
  //  }



  std::string target =
    "section .data\n"
//    "; int var\n"
    "var: dd 0\n"
    "section .text\n"
    "global main\n"
//    "; main function\n"
    "main:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
    "mov eax, [ebp+8]\n"
    "movzx eax, al\n"
    "push eax\n"
    "pop eax\n"
    "mov byte [ebp-1], al\n"
    "push dword [ebp+12]\n"
    "pop eax\n"
    "mov dword [ebp-8], eax\n"
    "push dword 5\n"
    "pop eax\n"
    "mov dword [ebp-12], eax\n"
    "push dword [ebp-12]\n"
    "pop eax\n"
    "mov dword [ebp+12], eax\n"
    "push dword 97\n"
    "pop eax\n"
    "mov byte [ebp+16], al\n"
    "push dword [ebp+12]\n"
    "pop eax\n"
    "mov dword [var], eax\n"
    "push dword 10\n"
    "pop eax\n"
    "mov dword [ebp+20], eax\n"
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "section .rodata\n";




  std::string file_name = "codegeneration/test_codegen_function_arguments3.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
  //  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));

}


TEST(codegen, functionCall) {

//  int test()
//  {
//    int a = 0;
//  }
//
//  int main()
//  {
//    test();
//  }

  std::string target =
    "section .data\n"
    "section .text\n"
    "global test\n"
//    "; test function\n"
    "test:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
    "push dword 0\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "global main\n"
//    "; main function\n"
    "main:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "lea ebx, [test]\n"
    "push ebx\n"
    "pop ebx\n"
    "mov ecx, ebx\n"
    "call ecx\n"
    "push eax\n"
    "pop eax\n"
     //fixme adde discard stack
    "push eax\n"
    "add esp, 4\n"
    "pop ebp\n"
    "ret\n"
    "section .rodata\n"
    ;


  std::string file_name = "codegeneration/test_codegen_function_call.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
  //  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));
}

TEST(codegen, functionCall2) {

  //  int test()
  //  {
  //    int a = 0;
  //  }
  //
  //  int main()
  //  {
  //    test();
  //  }


  std::string target =
    "section .data\n"
    "section .text\n"
    "global test\n"
    //    "; test function\n"
    "test:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
    "push dword 0\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "global main\n"
    //    "; main function\n"
    "main:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
    "push dword 0\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "lea ebx, [test]\n"
    "push ebx\n"
    "pop ebx\n"
    "mov ecx, ebx\n"
    "call ecx\n"
    "push eax\n"
    "pop eax\n"
    "movzx eax, al\n"
    //fixme adde discard stack
    "push eax\n"
    "add esp, 4\n"
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "section .rodata\n"
    ;




  std::string file_name = "codegeneration/test_codegen_function_call2.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
  //  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));

}

TEST(codegen, functionCall3) {


//  int printf(const char* format, ...);
//
//  int braze_test()
//  {
//    printf("hello world!");
//  }
//
//  int main()
//  {
//    braze_test();
//  }

  std::string target =
  "section .data\n"
  "section .text\n"
  "extern printf\n"
  "global braze_test\n"
//  "; braze_test function\n"
  "braze_test:\n"
  "push ebp\n"
  "mov ebp, esp\n"
  "lea ebx, [printf]\n"
  "push ebx\n"
  "pop ebx\n"
  "mov ecx, ebx\n"
  "mov eax, str_1\n"
  "push eax\n"
  "call ecx\n"
  "add esp, 4\n"
  "push eax\n"
  "pop eax\n"
  "push eax\n"
  "add esp, 4\n"
  "pop ebp\n"
  "ret\n"
  "global main\n"
//  "; main function\n"
  "main:\n"
  "push ebp\n"
  "mov ebp, esp\n"
  "lea ebx, [braze_test]\n"
  "push ebx\n"
  "pop ebx\n"
  "mov ecx, ebx\n"
  "call ecx\n"
  "push eax\n"
  "pop eax\n"
  "push eax\n"
  "add esp, 4\n"
  "pop ebp\n"
  "ret\n"
  "section .rodata\n"
  "str_1: db 'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', 0\n"
    ;


  std::string file_name = "codegeneration/test_codegen_function_call3.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
  //  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));
}


TEST(codegen, functionCall4) {


  //int function(int a, int b, char c, char d)
  //{
  //  int e = a;
  //  int f = b;
  //  c = 'c';
  //}
  //
  //int main()
  //{
  //  int g = 0;
  //
  //  char h = 'r';
  //  function(g, 20, h, 'f');
  //}

  std::string target =

    "section .data\n"
    "section .text\n"
    "global function\n"
    "function:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
    "push dword [ebp+8]\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "push dword [ebp+12]\n"
    "pop eax\n"
    "mov dword [ebp-8], eax\n"
    "push dword 99\n"
    "pop eax\n"
    "mov byte [ebp+16], al\n"
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "global main\n"
    "main:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
    "push dword 0\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "push dword 114\n"
    "pop eax\n"
    "mov byte [ebp-5], al\n"
    "lea ebx, [function]\n"
    "push ebx\n"
    "pop ebx\n"
    "mov ecx, ebx\n"
    "push dword 102\n"
    "mov eax, [ebp-5]\n"
    "movzx eax, al\n"
    "push eax\n"
    "push dword 20\n"
    "push dword [ebp-4]\n"
    "call ecx\n"
    "add esp, 16\n"
    "push eax\n"
    "pop eax\n"
    "push eax\n"
    "add esp, 4\n"
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "section .rodata\n"
    ;


  std::string file_name = "codegeneration/test_codegen_function_call4.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
  //  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));
  process.stop();

}

TEST(codegen, ifstatement) {


  //int main()
  //{
  //  if (1)
  //  {
  //    int a = 0;
  //  }
  //}

  std::string target =
    "section .data\n"
    "section .text\n"
    "global main\n"
    "main:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
    "push dword 1\n"
    "pop eax\n"
    "cmp eax, 0\n"
    "je .if_2\n"
    "push dword 0\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "jmp .if_end_1\n"
    ".if_2:\n"
    ".if_end_1:\n"
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "section .rodata\n"
    ;


  std::string file_name = "codegeneration/test_codegen_if_statement.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
  //  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));
  process.stop();
}



TEST(codegen, ifelsestatement) {


//  main()
    //{
    //  int a = 5;
    //  if (a > 10)
    //  {
    //    int a = 10;
    //  }
    //  else if (a < 10)
    //  {
    //    int b = a;
    //  }
    //  else
    //  {
    //    int c = 3;
    //  }
    //}

  std::string target =
    "section .data\n"
    "section .text\n"
    "global main\n"
    "main:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
    "push dword 5\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "push dword [ebp-4]\n"
    "push dword 10\n"
    "pop ecx\n"
    "pop eax\n"
    "cmp eax, ecx\n"
    "setg al\n"
    "movzx eax, al\n"
    "push eax\n"
    "pop eax\n"
    "cmp eax, 0\n"
    "je .if_2\n"
    "push dword 10\n"
    "pop eax\n"
    "mov dword [ebp-8], eax\n"
    "jmp .if_end_1\n"
    ".if_2:\n"
    "push dword [ebp-4]\n"
    "push dword 10\n"
    "pop ecx\n"
    "pop eax\n"
    "cmp eax, ecx\n"
    "setl al\n"
    "movzx eax, al\n"
    "push eax\n"
    "pop eax\n"
    "cmp eax, 0\n"
    "je .if_3\n"
    "push dword [ebp-4]\n"
    "pop eax\n"
    //"mov dword [ebp-8], eax\n"
    "mov dword [ebp-12], eax\n"
    "jmp .if_end_1\n"
    ".if_3:\n"
    "push dword 3\n"
    "pop eax\n"
    //"mov dword [ebp-8], eax\n"
    "mov dword [ebp-16], eax\n"
    ".if_end_1:\n"
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "section .rodata\n"
    ;


  std::string file_name = "codegeneration/test_codegen_if_else_statement.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
  //  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));

  process.stop();
}


TEST(codegen, whilestatement) {

//int main()
//{
//  while (1)
//  {
//    int a = 4;
//  }
//}
  std::string target =
    "section .data\n"
    "section .text\n"
    "global main\n"
    "main:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
//    ".entry_point_1:\n"
    ".while_start_1:\n"
    "push dword 1\n"
    "pop eax\n"
    "cmp eax, 0\n"
    "je .while_end_2\n"
    "push dword 4\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "jmp .while_start_1\n"
    ".while_end_2:\n"
//    ".exit_point_2:\n"
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "section .rodata\n"
    ;

  std::string file_name = "codegeneration/test_codegen_while_statement.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
  //  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));

  process.stop();
}



TEST(codegen, forstatement) {

  //int main()
  //{
  //  for (int i = 0; i < 10; i = i + 1)
  //  {
  //    int a = 5;
  //  }
  //}
  std::string target =
    "section .data\n"
    "section .text\n"
    "global main\n"
    "main:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
    "push dword 0\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "jmp .for_loop1\n"
//    ".entry_point_3:\n"
    "push dword [ebp-4]\n"
    "push dword 1\n"
    "pop ecx\n"
    "pop eax\n"
    "add eax, ecx\n"
    "push eax\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    ".for_loop1:\n"
    "push dword [ebp-4]\n"
    "push dword 10\n"
    "pop ecx\n"
    "pop eax\n"
    "cmp eax, ecx\n"
    "setl al\n"
    "movzx eax, al\n"
    "push eax\n"
    "pop eax\n"
    "cmp eax, 0\n"
    "je .for_loop_end2\n"
    "push dword 5\n"
    "pop eax\n"
    "mov dword [ebp-8], eax\n"
    "push dword [ebp-4]\n"
    "push dword 1\n"
    "pop ecx\n"
    "pop eax\n"
    "add eax, ecx\n"
    "push eax\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "jmp .for_loop1\n"
    ".for_loop_end2:\n"
//    ".exit_point_4:\n"
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "section .rodata\n"
    ;

  std::string file_name = "codegeneration/test_codegen_for_statement.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
  //  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));

  process.stop();
}



TEST(codegen, unary) {

  //int main()
  //{
  //  int* ptr;
  //  int val = 5;
  //  ptr = &val;
  //
  //  int result = *ptr;
  //}

  std::string target =
    "section .data\n"
    "section .text\n"
    "global main\n"
//    "; main function\n"
    "main:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
    "push dword 5\n"
    "pop eax\n"
    "mov dword [ebp-8], eax\n"
    "lea ebx, [ebp-8]\n"
    "push ebx\n"
    "pop ebx\n"
//    "; PUSH ADDRESS &\n"
    "push ebx\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "push dword [ebp-4]\n"
//    "; INDIRECTION\n"
    "pop ebx\n"
    "mov ebx, [ebx]\n"
    "push ebx\n"
    "pop eax\n"
    "mov dword [ebp-12], eax\n"
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "section .rodata\n"
    ;

  std::string file_name = "codegeneration/test_codegen_unary.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
  //  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));

  process.stop();
}




TEST(codegen, unary2) {

//  int main()
//  {
//    int a = 0;
//    int* ptr1 = &a;
//    int** ptr2 = &ptr1;
//    int c = **ptr2;
//  }


  std::string target =
    "section .data\n"
    "section .text\n"
    "global main\n"
//    "; main function\n"
    "main:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
    "push dword 0\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "lea ebx, [ebp-4]\n"
    "push ebx\n"
    "pop ebx\n"
//    "; PUSH ADDRESS &\n"
    "push ebx\n"
    "pop eax\n"
    "mov dword [ebp-8], eax\n"
    "lea ebx, [ebp-8]\n"
    "push ebx\n"
    "pop ebx\n"
//    "; PUSH ADDRESS &\n"
    "push ebx\n"
    "pop eax\n"
    "mov dword [ebp-12], eax\n"
    "push dword [ebp-12]\n"
//    "; INDIRECTION\n"
    "pop ebx\n"
    "mov ebx, [ebx]\n"
    "mov ebx, [ebx]\n"
    "push ebx\n"
    "pop eax\n"
    "mov dword [ebp-16], eax\n"
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "section .rodata\n"
    ;

  std::string file_name = "codegeneration/test_codegen_unary2.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
  //  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));

  process.stop();
}





TEST(codegen, unary3) {

//int test()
//{
//  int* a = 0xffc00000;
//  *a;
//}
//
//
//int main()
//{
//  test();
//}



  std::string target =

    "section .data\n"
    "section .text\n"
    "global test\n"
 //   "; test function\n"
    "test:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "sub esp, 16\n"
    "push dword 1744830464\n"
    "pop eax\n"
    "mov dword [ebp-4], eax\n"
    "push dword [ebp-4]\n"
//    "; INDIRECTION\n"
    "pop ebx\n"
    "mov ebx, [ebx]\n"
    "push ebx\n"
    "add esp, 4\n"
    "add esp, 16\n"
    "pop ebp\n"
    "ret\n"
    "global main\n"
//    "; main function\n"
    "main:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "lea ebx, [test]\n"
    "push ebx\n"
    "pop ebx\n"
    "mov ecx, ebx\n"
    "call ecx\n"
    "push eax\n"
    "pop eax\n"
    "push eax\n"
    "add esp, 4\n"
    "pop ebp\n"
    "ret\n"
    "section .rodata\n"
    ;

  std::string file_name = "codegeneration/test_codegen_unary3.c";
  std::string asm_file = file_name + ".asm";

  const int num_of_tokens = 5;

  compileProcess process;
  process.initialize(file_path + file_name);
  process.startCompiler();
  //  compareFiles(target, file_path + asm_file);
  EXPECT_TRUE(compareFiles(target, file_path + asm_file));

  process.stop();
}

