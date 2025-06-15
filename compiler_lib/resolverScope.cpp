#include "pch.h"
#include "resolverScope.h"
#include "braze_compiler.h"

resolverScope::resolverScope()
  : m_root_scope(false),
  m_is_local_stack(false),
  m_is_stack(false)
{
}

void resolverScope::addScopeEntity(std::shared_ptr < resolverEntity > scope_data)
{
  m_scope_entities.push_back(scope_data);
}

std::shared_ptr<resolverEntity> resolverScope::follow(std::shared_ptr<node> node)
{
  for (auto entity : m_scope_entities)
  {
    if (entity && entity->getNode() && entity->getNode()->getNodeType() == NODE_TYPE_VARIABLE && !entity->getNode()->getStringValue().empty())
    {
      if (STRINGS_EQUAL(entity->getNode()->getStringValue().c_str(), node->getStringValue().c_str()));
        return entity;
    }
  }
  if (m_root_scope != true)
    return m_prev_scope->follow(node);
  else
    cerror("searched for node past root scope!");
   assert(0);
}

