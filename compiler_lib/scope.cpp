#include "pch.h"
#include "scope.h"
#include "braze_compiler.h"

scope::scope()
  :m_size(0),
  m_flags(0),
  m_is_root_scope(false)
{

}

void scope::init(std::shared_ptr<scope> last_scope, bool is_root_scope)
{
  m_is_root_scope = is_root_scope;

  if (is_root_scope)
  {
    return;
  }
  m_parent = last_scope;
}

void scope::addNode(std::shared_ptr<node> node)
{
  std::shared_ptr < scopeEntity> scope_entity = std::make_shared < scopeEntity >();
  scope_entity->setNode(node);
  m_scope_entities.push_back(scope_entity);
  m_size += scope_entity->getStackOffset();
}

void scope::updateStackOffset(std::shared_ptr<node> node, int stack_offset)
{
  for (auto it = m_scope_entities.begin(); it != m_scope_entities.end(); it++)
  {
    std::shared_ptr < scopeEntity> scope_entity = (*it);
    if (node == scope_entity->getNode() && scope_entity->getNode())
    {
      m_size = stack_offset;
      scope_entity->setStackOffset(stack_offset);
      return;
    }
  }
  if (!m_is_root_scope)
  {
    m_parent->updateStackOffset(node, stack_offset);
    return;
  }
  cerror("Could not update stack offset on node", node->getFilePosition());
}
