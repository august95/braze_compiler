#pragma once

#include "../braze_compiler.h"
#include <list>
#include <string>

class preProcessor
{
public:
  preProcessor();
  void setTokenList(std::shared_ptr < std::list < std::shared_ptr<token > > >  tokens) { m_tokens_original = tokens; };
  std::shared_ptr < std::list < std::shared_ptr < token > > > getPreProcessedTokens() { return m_tokens_pre_processed; }
  int  startPreProcessor();

private:
  std::shared_ptr < std::list < std::shared_ptr < token > > > m_tokens_original;
  std::shared_ptr < std::list < std::shared_ptr < token > > > m_tokens_pre_processed;
  bool m_enabled;
};