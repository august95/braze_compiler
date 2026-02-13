#pragma once
#include "node.h"
#include "nodeExpression.h"


class nodeVariableDeclaration :
    public node
{
public:
  nodeVariableDeclaration();
  nodeVariableDeclaration(nodeType node_type, filePosition file_position);

  virtual void calculateStackOffset(int& stack_offset) override;

  virtual int getDatatypeSize() override;
  void setDatatype(std::shared_ptr<datatype> dtype) { m_datatype = dtype; }
  virtual std::shared_ptr<datatype> getDatatype() override { return m_datatype; }

  virtual void setStackOffset(int stack_offset) { m_stack_offset = stack_offset; }
  int getStackOffset() { return m_stack_offset; }

  void setValueNode(std::shared_ptr<nodeExpression> val_node) { m_value_node = val_node; }
  std::shared_ptr<nodeExpression> getValueNode() { return m_value_node; }
  void setIsFunctionArgument(bool is_function_argument) { m_is_function_argument = is_function_argument; }
  bool getIsFunctionArgument() { return m_is_function_argument; }
  virtual void setIsGlobal(bool is_global) override { m_is_global = is_global; }
  bool getIsGlobal() { return m_is_global; }

  virtual void setStringValue(std::string string_value) { m_string_value = string_value; node::setStringValue(string_value); }
  std::string getStringValue() { return m_string_value; }

private:
  std::string m_string_value;
  std::shared_ptr<nodeExpression> m_value_node;
  std::shared_ptr<datatype> m_datatype;
  int m_stack_offset;
  bool m_is_global;
  bool m_is_function_argument;
  int m_padding;
};

