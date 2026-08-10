#pragma once

#include "../braze_compiler.h"
#include "../node_/nodePreProcessor.h"
#include <list>
#include <memory>


class preProcessorExpressionable
{
public:
  preProcessorExpressionable();
  std::shared_ptr<token> nextToken();
  std::shared_ptr<token> peekToken();
  void pushNode(std::shared_ptr<nodePreProcessor> node);
  std::shared_ptr<nodePreProcessor> peekLastNode();
  std::shared_ptr<nodePreProcessor> popLastNode();


  std::list< std::shared_ptr<nodePreProcessor> > getAbstractSyntaxTree() { return m_nodes; }
  void setTokens(std::shared_ptr < std::list<std::shared_ptr<token> > > tokens) { m_tokens = tokens; }
  std::shared_ptr < std::list <std::shared_ptr<token> > > getTokens() { return m_tokens; }


private:
  std::shared_ptr < std::list<std::shared_ptr<token> > > m_tokens;
  std::list< std::shared_ptr<nodePreProcessor> > m_nodes;
};

