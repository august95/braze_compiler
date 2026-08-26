#pragma once

#include "../braze_compiler.h"
#include "preprocessorDefinition.h"
#include <list>
#include <string>
#include "preprocessorExpressionableParser.h"

class preProcessor
{
public:
  preProcessor();
  void setTokenList(std::shared_ptr < std::list < std::shared_ptr<token > > >  tokens) { m_tokens_original = tokens; };
  std::shared_ptr < std::list < std::shared_ptr < token > > > getPreProcessedTokens() { return m_tokens_pre_processed; }
  int  startPreProcessor();
  void setFileName(std::string file_name) { m_file_name = file_name; }

private:
  std::shared_ptr<token> nextToken();
  std::shared_ptr<token> peekToken();
  void pushToken(std::shared_ptr<token> token);

  void pushPreProcessedToken(std::shared_ptr<token> token);

  void handleToken();
  void handleIdentifier();
  void handleSymbol();
  bool handleHashtagToken();
  void handleIfToken();
  void handleDefinitionToken();
  void handleIfDef();
  void handleIfNDef();
  void undef();
  void handleWarning();
  void handleError();
  void handleInclude();
  std::list < std::shared_ptr<token> > handleDefinitionValue();
  void skipToEndif();
  void readToEndif(bool condition);
  int parseAndEvaluate();
  std::string generateIncludePath(std::string include_string);

  class definitions
  {
  public:
    void addDefinition(std::string name, std::shared_ptr < std::list<std::shared_ptr<token>>> value_tokens, std::shared_ptr < std::list<std::shared_ptr<token>>> argument_tokens);
    void removeDefinition(std::string name);
    std::shared_ptr<preProcessorDefinition> getDefintion(std::string name);
  private:
    std::list < std::shared_ptr<preProcessorDefinition> > m_defintions;
  };
private:
  definitions m_definitions;
  std::shared_ptr < std::list < std::shared_ptr < token > > > m_tokens_original;
  std::shared_ptr < std::list < std::shared_ptr < token > > > m_tokens_pre_processed;
  preProcessorExpressionableParser m_preprocessor_expressionable_parser;
  std::list<std::string> m_include_paths;
  friend preProcessorExpressionableParser;
  friend nodePreProcessor;
  std::string m_file_name;
  std::string m_file_path;
  bool m_enabled;
};