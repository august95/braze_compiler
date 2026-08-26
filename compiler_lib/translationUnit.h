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
  int startIncludeCompilation();
  void stop();

  // for unit tests
  std::shared_ptr < std::list < std::shared_ptr < token > > > getTokens();
  std::list<std::shared_ptr<node>> getAbstractSyntaxTree() { return m_parser.getAbstractSyntaxTree(); }

  bool __no_code_generation;

private:
  lexer m_lexer;
  parser m_parser;
  preProcessor m_pre_processor;
  std::shared_ptr<codeGenerator> m_code_generator;
};
