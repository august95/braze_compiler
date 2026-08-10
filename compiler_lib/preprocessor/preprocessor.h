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

private:
  std::shared_ptr<token> nextToken();
  std::shared_ptr<token> peekToken();
  void pushToken(std::shared_ptr<token> token);

  void pushPreProcessedToken(std::shared_ptr<token> token);

  void handleToken();
  void handleSymbol();
  bool handleHashtagToken();
  void handleIfToken();
  void handleDefinitionToken();
  std::list < std::shared_ptr<token> > handleDefinitionValue();
  void skipToEndif();
  void readToEndif(bool condition);
  int parseAndEvaluate();

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
  friend preProcessorExpressionableParser;
  friend nodePreProcessor;
  bool m_enabled;
};