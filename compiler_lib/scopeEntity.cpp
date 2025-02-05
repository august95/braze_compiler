#include "pch.h"
#include "scopeEntity.h"

scopeEntity::scopeEntity()
  :m_flags(0),
  m_stack_offset(0)
{
}

std::shared_ptr<node> scopeEntity::getNode()
{
  return m_node;
}

void scopeEntity::setNode(std::shared_ptr<node> node)
{
  m_node = node;
}

void scopeEntity::setStackOffset(int stack_offset)
{
  if (m_node)
  {
    m_node->setStackOffset(stack_offset);
  }
  m_stack_offset = stack_offset;
}
