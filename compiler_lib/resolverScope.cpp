#include "pch.h"
#include "resolverScope.h"

resolverScope::resolverScope()
  : m_root_scope(false),
  m_is_local_stack(false),
  m_is_stack(false)
{
}

void resolverScope::addScopeData(std::shared_ptr < resolverEntity > scope_data)
{
  m_scope_data.push_back(scope_data);
}

std::shared_ptr<resolverEntity> resolverScope::follow(std::shared_ptr<node> node)
{
  for (auto entity : m_scope_data)
  {
    if (entity && entity->getNode() && entity->getNode()->getNodeType() == NODE_TYPE_VARIABLE && !entity->getNode()->getStringValue().empty())
    {
      if (STRINGS_EQUAL(entity->getNode()->getStringValue().c_str(), node->getStringValue().c_str()));
        return entity;
    }
  }
}

