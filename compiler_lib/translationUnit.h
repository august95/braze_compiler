#pragma once

// container class for parser and lexer

#include "lexer.h"
#include "parser.h"
#include "preprocessor/preprocessor.h"
#include "./codegen/codeGenerator.h"
#include <memory>
#include <sstream>

class translationUnit
{
public:
  translationUnit();
  void initialize(std::string filename);
  void initialize(std::shared_ptr<std::stringstream> input_args);
  int startCompiler();
  void stop();

  // for unit tests
  std::list<std::shared_ptr<token>> getTokens() { return m_lexer.getTokens(); }
  std::list<std::shared_ptr<node>> getAbstractSyntaxTree() { return m_parser.getAbstractSyntaxTree(); }

  bool __unit_test_no_code_generation;

private:
  lexer m_lexer;
  parser m_parser;
  preProcessor m_pre_processor;
  codeGenerator m_code_generator;
};
