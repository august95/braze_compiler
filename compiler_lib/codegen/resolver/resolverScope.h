#pragma once
#include "resolverEntity.h"
#include "resolverResult.h"
#include "../../node_/nodeExpression.h"
#include <list>
#include <memory>

class resolverScope
{
public:
  explicit resolverScope(std::shared_ptr<resolverScope> parent = nullptr);
  std::shared_ptr<resolverScope> getParent() const { return m_parent; }
  void addScopeEntity(std::shared_ptr<resolverEntity> scope_data);
  void follow(std::shared_ptr<node> node, std::shared_ptr<resolverResult> result);
  void followName(std::shared_ptr<node> node, std::shared_ptr<resolverResult> result);
  void followExpression(std::shared_ptr<nodeExpression> node, std::shared_ptr<resolverResult> result);
  void followFunctionCall(std::shared_ptr<nodeExpression> node_, std::shared_ptr<resolverResult> result);
  void followUnary(std::shared_ptr<nodeExpression> node_, std::shared_ptr<resolverResult> result);
  void followUnaryAddress(std::shared_ptr<nodeExpression> node_, std::shared_ptr<resolverResult> result);
  void followUnaryIndirection(std::shared_ptr<nodeExpression> node_, std::shared_ptr<resolverResult> result);
  int buildFunctionCallArguments(std::shared_ptr<nodeExpression> node, std::shared_ptr<resolverEntity> function_call_entity);

private:
  std::shared_ptr<resolverScope> m_parent;
  std::list<std::shared_ptr<resolverEntity>> m_scope_entities;
};
