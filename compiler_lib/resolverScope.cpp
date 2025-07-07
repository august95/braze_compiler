#include "pch.h"
#include "resolverScope.h"
#include "braze_compiler.h"

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

