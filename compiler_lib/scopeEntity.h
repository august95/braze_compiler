#pragma once
#include "node.h"
class scopeEntity
{
public:
  scopeEntity();
  std::shared_ptr < node > getNode();
  void setNode(std::shared_ptr < node > node);
  int getStackOffset() { return m_stack_offset; }
  void setStackOffset(int stack_offset);
private:
  int m_flags;
  int m_stack_offset;
  std::shared_ptr < node > m_node;
  };

