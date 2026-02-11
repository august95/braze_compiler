#pragma once
#include "node.h"
#include "../datatype.h"
#include <iostream>

#define EXPRESSION_GEN_MATHABLE (      \
    EXPRESSION_IS_ADDITION |           \
    EXPRESSION_IS_SUBTRACTION |        \
    EXPRESSION_IS_MULTPILICATION |     \
    EXPRESSION_IS_DIVISION |           \
    EXPRESSION_IS_MODULUS |            \
    EXPRESSION_IS_FUNCTION_CALL |      \
    EXPRESSION_INDIRECTION |           \
    EXPRESSION_GET_ADDRESS |           \
    EXPRESSION_IS_ABOVE |              \
    EXPRESSION_IS_ABOVE_OR_EQUAL |     \
    EXPRESSION_IS_BELOW |              \
    EXPRESSION_IS_BELOW_OR_EQUAL |     \
    EXPRESSION_IS_EQUAL |              \
    EXPRESSION_IS_NOT_EQUAL |          \
    EXPRESSION_LOGICAL_AND |           \
    EXPRESSION_LOGICAL_OR |            \
    EXPRESSION_IN_LOGICAL_EXPRESSION | \
    EXPRESSION_BITSHIFT_LEFT |         \
    EXPRESSION_BITSHIFT_RIGHT |        \
    EXPRESSION_IS_BITWISE_OR |         \
    EXPRESSION_IS_BITWISE_AND |        \
    EXPRESSION_IS_BITWISE_XOR)

#define EXPRESSION_LOGICAL_OPERATOR ( \
    EXPRESSION_LOGICAL_AND |          \
    EXPRESSION_LOGICAL_OR)

enum ExpressionType
{
  EXPRESSION_FLAG_NONE = 0b00000000000000000000000000000000,
  EXPRESSION_FLAG_RIGH_NODE = 0b00000000000000000000000000000001,
  EXPRESSION_IN_FUNCTION_CALL_ARGUMENTS = 0b00000000000000000000000000000010,
  EXPRESSION_IN_FUNCTION_CALL_LEFT_OPERAND = 0b00000000000000000000000000000100,
  EXPRESSION_IS_ADDITION = 0b00000000000000000000000000001000,
  EXPRESSION_IS_SUBTRACTION = 0b00000000000000000000000000010000,
  EXPRESSION_IS_MULTPILICATION = 0b00000000000000000000000000100000,
  EXPRESSION_IS_DIVISION = 0b00000000000000000000000001000000,
  EXPRESSION_IS_FUNCTION_CALL = 0b00000000000000000000000010000000,
  EXPRESSION_INDIRECTION = 0b00000000000000000000000100000000,
  EXPRESSION_GET_ADDRESS = 0b00000000000000000000001000000000,
  EXPRESSION_IS_ABOVE = 0b00000000000000000000010000000000,
  EXPRESSION_IS_ABOVE_OR_EQUAL = 0b00000000000000000000100000000000,
  EXPRESSION_IS_BELOW = 0b00000000000000000001000000000000,
  EXPRESSION_IS_BELOW_OR_EQUAL = 0b00000000000000000010000000000000,
  EXPRESSION_IS_EQUAL = 0b00000000000000000100000000000000,
  EXPRESSION_IS_NOT_EQUAL = 0b00000000000000001000000000000000,
  EXPRESSION_LOGICAL_AND = 0b00000000000000010000000000000000,
  EXPRESSION_LOGICAL_OR = 0b00000000000000100000000000000000,
  EXPRESSION_IN_LOGICAL_EXPRESSION = 0b00000000000001000000000000000000,
  EXPRESSION_BITSHIFT_LEFT = 0b00000000000010000000000000000000,
  EXPRESSION_BITSHIFT_RIGHT = 0b00000000000100000000000000000000,
  EXPRESSION_IS_BITWISE_OR = 0b00000000001000000000000000000000,
  EXPRESSION_IS_BITWISE_AND = 0b00000000010000000000000000000000,
  EXPRESSION_IS_BITWISE_XOR = 0b00000000100000000000000000000000,
  EXPRESSION_IS_NOT_ROOT_NODE = 0b00000001000000000000000000000000,
  EXPRESSION_IS_ASSIGNMENT = 0b00000010000000000000000000000000,
  IS_ALONE_STATEMENT = 0b00000100000000000000000000000000,
  EXPRESSION_IS_UNARY = 0b00001000000000000000000000000000,
  IS_STATEMENT_RETURN = 0b00010000000000000000000000000000,
  IS_RIGHT_OPERAND_OF_ASSIGNMENT = 0b00100000000000000000000000000000,
  IS_LEFT_OPERAND_OF_ASSIGNMENT = 0b01000000000000000000000000000000,
  EXPRESSION_IS_MODULUS = 0b10000000000000000000000000000000,
};

class nodeExpression : public node
{
public:
  nodeExpression();
  nodeExpression(filePosition file_position);
  nodeExpression(nodeType node_type, filePosition file_position);
  ~nodeExpression();

  ExpressionType getExpressionType() { return m_exp_type; }

  void setParenthesesNode(std::shared_ptr<nodeExpression> node) { m_parentheses_node = node; }
  std::shared_ptr<nodeExpression> getParenthesesNode() { return m_parentheses_node; }
  std::shared_ptr<nodeExpression> getLeftNode() { return m_left_node; }
  void setLeftNode(std::shared_ptr<nodeExpression> left_node) { m_left_node = left_node; }
  std::shared_ptr<nodeExpression> getRightNode() { return m_right_node; }
  void setRightNode(std::shared_ptr<nodeExpression> right_node) { m_right_node = right_node; }
  virtual void setDatatype(std::shared_ptr<datatype> dtype) override { m_datatype_exp = dtype; node::setDatatype(dtype); } //FIXME resolve dtype owership with other datatype owners}
  std::shared_ptr<datatype> getDatatype() { return m_datatype_exp; }
  virtual void setStringValue(std::string string_value) override { m_string_value_exp = string_value; node::setStringValue(string_value); } //FIXME resolve dtype owership with other datatype owners
  std::string getStringValue() { return m_string_value_exp; }
  void setNumberValue(unsigned long number_val) { m_number_val = number_val; }
  unsigned long getNumberValue() { return m_number_val; }

  void generateExpressionFlag();

  bool isArray() { return m_node_type == nodeType::NODE_TYPE_EXPRESSION && m_string_value_exp == "[]"; }
  bool isAssignmentNode() { return m_node_type == nodeType::NODE_TYPE_EXPRESSION && (m_string_value_exp == "=" || m_string_value_exp == "+=" || m_string_value_exp == "-=" || m_string_value_exp == "/=" || m_string_value_exp == "*="); }
  bool isValueNode() { return m_node_type == NODE_TYPE_IDENTIFIER || m_node_type == NODE_TYPE_NUMBER || m_node_type == NODE_TYPE_UNARY || m_node_type == NODE_TYPE_TENARY || m_node_type == NODE_TYPE_STRING; }
private:

  std::shared_ptr<nodeExpression> m_parentheses_node;

  // for expression nodes
  std::shared_ptr<nodeExpression> m_left_node;
  std::shared_ptr<nodeExpression> m_right_node;
  std::shared_ptr<datatype> m_datatype_exp;
  ExpressionType m_exp_type;

  std::string m_string_value_exp;
  unsigned long m_number_val;
};