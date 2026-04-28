#pragma once
#include "node.h"
#include "../datatype.h"
#include "nodeExpression.h"
#include "nodeVariableDeclaration.h"
#include "nodeBody.h"


class nodeStatement :
    public node
{
public:
  nodeStatement();
  nodeStatement(nodeType node_type, filePosition file_position);

  virtual void calculateStackOffset(int& stack_offset) override;

  void setConditionNode(std::shared_ptr<nodeExpression> condition_node) { m_condition_node = condition_node; }
  std::shared_ptr<nodeExpression> getConditionNode() { return m_condition_node; }
  void setNextElseNode(std::shared_ptr<nodeStatement> next_else_node) { m_next_else_node = next_else_node; }
  std::shared_ptr<nodeStatement> getNextElseNode() { return m_next_else_node; }
  void setInitNode(std::shared_ptr<nodeVariableDeclaration> init_node) { m_init_node = init_node; }
  std::shared_ptr<nodeVariableDeclaration> getInitNode() { return m_init_node; }
  void setLoopNode(std::shared_ptr<node> loop_node) { m_loop_node = loop_node; }
  std::shared_ptr<node> getLoopNode() { return m_loop_node; }
  void setReturnValueNode(std::shared_ptr<nodeExpression> return_value_node) { m_return_value_node = return_value_node; }
  std::shared_ptr<nodeExpression> getReturnValueNode() { return m_return_value_node; }
  void setBodyNode(std::shared_ptr<nodeBody> body_node) { m_body_node = body_node; }
  std::shared_ptr<nodeBody> getBodyNode() { return m_body_node; }
private:


  std::shared_ptr<nodeExpression> m_condition_node;
  std::shared_ptr<nodeStatement> m_next_else_node;
  std::shared_ptr<nodeVariableDeclaration> m_init_node;
  std::shared_ptr<node> m_loop_node;
  std::shared_ptr<nodeExpression> m_return_value_node;
  std::shared_ptr<nodeBody> m_body_node;
};

