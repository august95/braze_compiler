#pragma once
// lexing source code and creates a collectino of tokens that represents the content in the source code.

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <memory>

#include "filePosition.h"
#include "token.h"
#include "charstream/charStreamInterface.h"

class lexer
{
public:
  lexer();
  ~lexer();

  // TODO: these functions and ifstream should be provided by an interface(made by template?)
  // the lexer should not own the file stream

  int startLexer();
  std::list<std::shared_ptr<token>> getTokens() { return tokens; }
  void setCharStreamInterface(std::shared_ptr<charStreamInterface> char_stream) { m_char_stream = char_stream; }

private:
  void lexFile();
  std::shared_ptr<token> readNextToken();
  void handleComment();
  bool handleComment_();
  void handleSingleLineComment();
  void handleMultiLineComment();

  std::shared_ptr<token> makeIdentifierOrKeyword();
  bool is_keyword(std::string _keyword_);

  std::shared_ptr<token> makeOperatorTokenOrIncludeString();
  std::string getOperatorString();
  bool isOperatorValid(std::string _operator_);
  bool lastTokenIsInlcudeKeyword();
  std::shared_ptr<token> makeStringToken();
  std::string createString(char start_char, char end_char);

  std::shared_ptr<token> makeSymbolToken();
  std::shared_ptr<token> makeQuoteToken();
  std::shared_ptr<token> makeNewLineToken();

  std::shared_ptr<token> makeNumberToken();
  std::shared_ptr<token> makeHexicalNumberToken();
  std::shared_ptr<token> makeBinaryNumberToken();
  std::shared_ptr<token> makeDecimalNumberToken();
  bool isHexChar(char c);
  std::shared_ptr<token> handle_whitespace();

  filePosition getFilePostiion() { return m_char_stream->getFilePostiion(); }


  std::shared_ptr<charStreamInterface> m_char_stream;
  // list of tokens that has been lexed. Will be handed over to the parser
  std::list<std::shared_ptr<token>> tokens;

  void _assert_(bool condition, std::string message);
};
