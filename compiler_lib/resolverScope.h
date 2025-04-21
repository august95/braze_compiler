#pragma once
#include <iostream>
#include <list>
#include "resolverScopeData.h"

class resolverScope
{
public:
  resolverScope();
  void setNextScope(std::shared_ptr <resolverScope> next_scope) { m_next_scope = next_scope; }
  std::shared_ptr < resolverScope> getNextScope() { return m_next_scope; }
  bool isRootScope() { return m_root_scope; }
  void setRootScope(bool root_scope) { m_root_scope = root_scope; }
  void addScopeData(std::shared_ptr < resolverEntity > scope_data);
  bool isLocalStack() { return m_is_local_stack; }
  void setLocalStack(bool is_local_stack) { m_is_local_stack = is_local_stack; }
  bool isStack() { return m_is_stack; }
  void setStack(bool is_stack) { m_is_stack = is_stack; }
  std::shared_ptr < resolverEntity > follow(std::shared_ptr < node > node);
private:
  std::shared_ptr <resolverScope> m_next_scope;
  std::shared_ptr <resolverScope> m_prev_scope;
  std::list < std::shared_ptr < resolverEntity > > m_scope_data;
  bool m_is_local_stack;
  bool m_is_stack;
  bool m_root_scope;
};

