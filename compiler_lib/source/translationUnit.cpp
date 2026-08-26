#include "pch.h"
#include "../translationUnit.h"
#include "../braze_compiler.h"
#include "../charstream/charStreamFile.h"
#include "../charstream/charStreamArgs.h"
#include "../codegen/asmWriter.h"
#include <cstdio> // For fopen, fclose, fread, fwrite, etc.

translationUnit::translationUnit()
  : __no_code_generation(false)
{
}

void translationUnit::initialize(std::string filename)
{
  std::shared_ptr<charStreamFile> char_stream = std::make_shared <charStreamFile>();
  char_stream->initialize(filename);
  m_lexer.setCharStreamInterface(char_stream);
  if (!__no_code_generation)
  {
    m_code_generator = std::make_shared<codeGenerator>();
    m_code_generator->setFileName(filename);
  }
  m_pre_processor.setFileName(filename);
}

void translationUnit::initialize(std::shared_ptr<std::stringstream> input_args)
{
  std::shared_ptr<charStreamArgs> char_stream = std::make_shared <charStreamArgs>();
  char_stream->initialize(input_args);
  m_lexer.setCharStreamInterface(char_stream);
  m_code_generator->setFileName("app.out", asmWriter::WriteMode::W_STDOUT);
}

int translationUnit::startCompiler()
{
  int ret = m_lexer.startLexer();
  if (ret != 0)
  {
    cerror("failed to lex file!");
    return ret;
  }

  m_pre_processor.setTokenList(std::make_shared< std::list<std::shared_ptr<token> > > (m_lexer.getTokens()));
  ret = m_pre_processor.startPreProcessor();
  if (ret != 0)
  {
    cerror("failed to preprocess file!");
    return ret;
  }

  m_parser.setTokenList(m_pre_processor.getPreProcessedTokens());
  ret = m_parser.startParser();
  if (ret != 0)
  {
    cerror("failed to parse tokens into abstract syntax tree!");
    return ret;
  }

  if (__no_code_generation)
    return 0;
  
  m_code_generator->setAbstractSyntaxTree(m_parser.getAbstractSyntaxTree());
  ret = m_code_generator->startCodeGeneration();
  if (ret != 0)
  {
    cerror("failed to generate code from abstract syntax tree!");
    return ret;
  }
  return ret;

}

int translationUnit::startIncludeCompilation()
{
  int ret = m_lexer.startLexer();
  if (ret != 0)
  {
    cerror("failed to lex file!");
    return ret;
  }

  m_pre_processor.setTokenList(std::make_shared< std::list<std::shared_ptr<token> > >(m_lexer.getTokens()));
  ret = m_pre_processor.startPreProcessor();
  if (ret != 0)
  {
    cerror("failed to preprocess file!");
    return ret;
  }
  return ret;
}

void translationUnit::stop()
{
  m_code_generator->generateLableCount(true);
}

std::shared_ptr < std::list < std::shared_ptr < token > > > translationUnit::getTokens()
{ 
  return m_pre_processor.getPreProcessedTokens();
}