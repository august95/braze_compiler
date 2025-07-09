#include "pch.h"
#include "../resolverScope.h"
#include "../braze_compiler.h"

resolverScope::resolverScope()
  : m_root_scope(false),
  m_is_local_stack(false),
  m_is_stack(false),
  flags(false)
{
}

void resolverScope::addScopeEntity(std::shared_ptr < resolverEntity > scope_data)
{
  m_scope_entities.push_back(scope_data);
}

void resolverScope::follow(std::shared_ptr<node> node, std::shared_ptr<resolverResult> result)
{
  if (node->getNodeType() == NODE_TYPE_VARIABLE)
  {
    followName(node, result);
  }
  else if (node->getNodeType() == NODE_TYPE_IDENTIFIER)
  {
    followName(node, result);
  }
  else if (node->getNodeType() == NODE_TYPE_EXPRESSION)
  {
    followExpression(node, result);
  }
  else if (node->getNodeType() == NODE_TYPE_EXPRESSION_PARANTHESES)
  {
    followExpression(node->getParenthesesNode() , result);
  }
}

void resolverScope::followName(std::shared_ptr<node> node, std::shared_ptr<resolverResult> result)
{
  for (auto entity : m_scope_entities)
  {
    if (!entity->getNode()->getStringValue().empty())
    {
      std::string entity_s = entity->getNode()->getStringValue().c_str();
      std::string node_s = node->getStringValue().c_str();
      if (STRINGS_EQUAL(entity_s.c_str(), node_s.c_str()))
      {
        result->addEntity(entity);
        return;
      }
    }
  }
  if (m_root_scope != true)
    return m_prev_scope->follow(node, result);
  else
    cerror("searched for node past root scope!");
  assert(0);
}

void resolverScope::followExpression(std::shared_ptr<node> node, std::shared_ptr<resolverResult> result)
{

  if (STRINGS_EQUAL(node->getStringValue().c_str(), "()"))
  {
    followFunctionCall(node, result);
  }
}

void resolverScope::followFunctionCall(std::shared_ptr<node> node_, std::shared_ptr<resolverResult> result)
{
  assert(node_->getLeftNode()->getNodeType() == NODE_TYPE_IDENTIFIER);
  std::shared_ptr< node > func_name = node_->getLeftNode();

  follow(func_name, result);
  std::shared_ptr < resolverEntity > function_entity = result->peekEntity();
  if (!function_entity)
  {
    cerror("could not resolve function!");
  }
  std::shared_ptr < resolverEntity > function_call_entity = std::make_shared < resolverEntity >();
  function_call_entity->setEntityType(E_FUNCTION_CALL);
  result->addEntity(function_call_entity);
  
}

