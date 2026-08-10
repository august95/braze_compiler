#pragma once
#include "node.h"
#include <iostream>


class nodePreProcessor : public node
{
public:
  nodePreProcessor( nodeType node_type, filePosition file_position);
  int evaluate();
  int evaluateIdentifier();
  int evaluateExpression();
  int evaluateUnary();
  int evaluateNumber();

private:
public:
  void setNumberValue(unsigned long number_val) { m_number_val = number_val; }
  unsigned long getNumberValue() { return m_number_val; }
  std::shared_ptr<nodePreProcessor> getLeftNode() { return m_left_node; }
  void setLeftNode(std::shared_ptr<nodePreProcessor> left_node) { m_left_node = left_node; }
  std::shared_ptr<nodePreProcessor> getRightNode() { return m_right_node; }
  void setRightNode(std::shared_ptr<nodePreProcessor> right_node) { m_right_node = right_node; }
  virtual void calculateStackOffset(int& stack_offset) {};
  void setPreProcessor(class preProcessor* preprocessor);

private:

  int arithmetic(std::string operator_, long left_operand, long right_operand);

  class preProcessor* m_preprocessor;
  std::shared_ptr<nodePreProcessor> m_left_node;
  std::shared_ptr<nodePreProcessor> m_right_node;
  unsigned long m_number_val;
};