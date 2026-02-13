#pragma once
#include "resolverEntity.h"
#include "resolverResult.h"
#include "../../node_/nodeExpression.h"
#include <iostream>
#include <list>
#include <memory>

class resolverScope
{
public:
  resolverScope();
  void setNextScope(std::shared_ptr<resolverScope> next_scope) { m_next_scope = next_scope; }
  void setPrevScope(std::shared_ptr<resolverScope> prev_scope) { m_prev_scope = prev_scope; }
  std::shared_ptr<resolverScope> getPrevScope() { return m_prev_scope; }
  std::shared_ptr<resolverScope> getNextScope() { return m_next_scope; }

  bool isRootScope() { return m_root_scope; }
  void setRootScope(bool root_scope) { m_root_scope = root_scope; }
  void addScopeEntity(std::shared_ptr<resolverEntity> scope_data);
  bool isLocalStack() { return m_is_local_stack; }
  void setLocalStack(bool is_local_stack) { m_is_local_stack = is_local_stack; }
  bool isStack() { return m_is_stack; }
  void setStack(bool is_stack) { m_is_stack = is_stack; }
  void follow(std::shared_ptr<node> node, std::shared_ptr<resolverResult> result);
  void followName(std::shared_ptr<node> node, std::shared_ptr<resolverResult> result);
  void followExpression(std::shared_ptr<nodeExpression> node, std::shared_ptr<resolverResult> result);
  void followFunctionCall(std::shared_ptr<nodeExpression> node_, std::shared_ptr<resolverResult> result);
  void followUnary(std::shared_ptr<nodeExpression> node_, std::shared_ptr<resolverResult> result);
  void followUnaryAddress(std::shared_ptr<nodeExpression> node_, std::shared_ptr<resolverResult> result);
  void followUnaryIndirection(std::shared_ptr<nodeExpression> node_, std::shared_ptr<resolverResult> result);
  void buildFunctionCallArguments(std::shared_ptr<nodeExpression> node_, std::shared_ptr<resolverEntity> function_call_entity, std::shared_ptr<resolverResult> result, int &function_call_stack_size);

private:
  std::shared_ptr<resolverScope> m_next_scope;
  std::shared_ptr<resolverScope> m_prev_scope;
  std::list<std::shared_ptr<resolverEntity>> m_scope_entities;
  bool m_is_local_stack;
  bool m_is_stack;
  bool m_root_scope;
  int flags;
};
