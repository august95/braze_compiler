#include "../preProcessorExpresssionable.h"

preProcessorExpressionable::preProcessorExpressionable()
{
}


std::shared_ptr<token> preProcessorExpressionable::nextToken()
{
  if (m_tokens->empty())
    return std::make_shared<token>();

  std::shared_ptr<token> token = m_tokens->front();
  m_tokens->pop_front();

  // ignore new line tokens
  while (!m_tokens->empty() && token->isTokenTypeNewLine())
  {
    token = m_tokens->front();
    m_tokens->pop_front();
  }
  return token;
}

std::shared_ptr<token> preProcessorExpressionable::peekToken()
{
  if (m_tokens->empty())
    return std::shared_ptr<token>();
  return m_tokens->front();
}
void preProcessorExpressionable::pushNode(std::shared_ptr<nodePreProcessor> node)
{
  m_nodes.push_back(node);
}

std::shared_ptr<nodePreProcessor> preProcessorExpressionable::peekLastNode()
{
  if (!m_nodes.empty())
    return m_nodes.back();
  return std::shared_ptr<nodePreProcessor>();
}
std::shared_ptr<nodePreProcessor> preProcessorExpressionable::popLastNode()
{
  auto node = m_nodes.back();
  m_nodes.pop_back();
  return node;
}