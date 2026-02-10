#include "../../source/pch.h"
#include "../nodeExpression.h"

nodeExpression::nodeExpression()
  :node(),
  m_number_val(0),
  m_exp_type(EXPRESSION_FLAG_NONE)
{
  m_node_type = NODE_TYPE_EXPRESSION;
}


nodeExpression::nodeExpression(filePosition file_position)
  :node(file_position),
  m_number_val(0),
  m_exp_type(EXPRESSION_FLAG_NONE)
{
  m_node_type = NODE_TYPE_EXPRESSION;
}

nodeExpression::nodeExpression(nodeType node_type, filePosition file_position)
  :node(node_type, file_position),
  m_number_val(0),
  m_exp_type(EXPRESSION_FLAG_NONE)
{
}

nodeExpression::~nodeExpression()
{

}

void nodeExpression::calculateStackOffset(int& stack_offset)
{

}

bool nodeExpression::isValidExpressionType()
{
  return m_node_type == NODE_TYPE_EXPRESSION ||
    m_node_type == NODE_TYPE_EXPRESSION_PARANTHESES ||
    m_node_type == NODE_TYPE_IDENTIFIER ||
    m_node_type == NODE_TYPE_UNARY ||
    m_node_type == NODE_TYPE_NUMBER ||
    m_node_type == NODE_TYPE_STRING;
}


void nodeExpression::generateExpressionFlag()
{
  if (STRINGS_EQUAL(m_string_value_exp.c_str(), "+"))
  {
    m_exp_type = EXPRESSION_IS_ADDITION;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), "-"))
  {
    m_exp_type = EXPRESSION_IS_SUBTRACTION;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), "*"))
  {
    m_exp_type = EXPRESSION_IS_MULTPILICATION;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), "/"))
  {
    m_exp_type = EXPRESSION_IS_DIVISION;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), "%"))
  {
    m_exp_type = EXPRESSION_IS_MODULUS;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), ">"))
  {
    m_exp_type = EXPRESSION_IS_ABOVE;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), "<"))
  {
    m_exp_type = EXPRESSION_IS_BELOW;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), ">="))
  {
    m_exp_type = EXPRESSION_IS_ABOVE_OR_EQUAL;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), "<="))
  {
    m_exp_type = EXPRESSION_IS_BELOW_OR_EQUAL;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), "!="))
  {
    m_exp_type = EXPRESSION_IS_NOT_EQUAL;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), "=="))
  {
    m_exp_type = EXPRESSION_IS_EQUAL;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), "&&"))
  {
    m_exp_type = EXPRESSION_LOGICAL_AND;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), "||"))
  {
    m_exp_type = EXPRESSION_LOGICAL_OR;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), "<<"))
  {
    m_exp_type = EXPRESSION_BITSHIFT_LEFT;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), ">>"))
  {
    m_exp_type = EXPRESSION_BITSHIFT_RIGHT;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), "&"))
  {
    m_exp_type = EXPRESSION_IS_BITWISE_AND;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), "|"))
  {
    m_exp_type = EXPRESSION_IS_BITWISE_OR;
  }
  else if (STRINGS_EQUAL(m_string_value_exp.c_str(), "^"))
  {
    m_exp_type = EXPRESSION_IS_BITWISE_XOR;
  }
}
