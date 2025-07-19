#include "pch.h"
#include "../node.h"
#include "../braze_compiler.h"

node::node()
    : m_body_size(0),
      m_node_type(nodeType::NODE_TYPE_BLANK),
      m_number_val(0),
      m_stack_offset(0),
      m_is_function_argument(0),
      m_is_global(0),
      m_exp_type(EXPRESSION_FLAG_NONE),
      m_stack_size(0),
      m_stack_addition(4),
      m_padding(0),
      m_function_prototype(0)
{
}

node::node(filePosition file_position)
    : m_file_position(file_position),
      m_node_type(nodeType::NODE_TYPE_BLANK),
      m_body_size(0),
      m_number_val(0),
      m_stack_offset(0),
      m_is_function_argument(0),
      m_is_global(0),
      m_exp_type(EXPRESSION_FLAG_NONE),
      m_stack_size(0),
      m_stack_addition(4),
      m_padding(0),
      m_function_prototype(0)
{
}

node::node(nodeType node_type, filePosition file_position)
    : m_node_type(node_type),
      m_file_position(file_position),
      m_body_size(0),
      m_number_val(0),
      m_stack_offset(0),
      m_is_function_argument(0),
      m_is_global(0),
      m_exp_type(EXPRESSION_FLAG_NONE),
      m_stack_size(0),
      m_stack_addition(4),
      m_padding(0),
      m_function_prototype(0)
{
}

bool node::isValidExpressionType()
{
  return m_node_type == NODE_TYPE_EXPRESSION ||
         m_node_type == NODE_TYPE_EXPRESSION_PARANTHESES ||
         m_node_type == NODE_TYPE_IDENTIFIER ||
         m_node_type == NODE_TYPE_UNARY ||
         m_node_type == NODE_TYPE_NUMBER ||
         m_node_type == NODE_TYPE_STRING;
}

int node::getDatatypeSize()
{
  // local variables in the scope increment the stack size of that scope,
  // function parameters is not yet parsed by the parser. The parameters
  // will be located above the previous base pointer on the stack. Will
  // probably require an own stack size function
  if (m_node_type == NODE_TYPE_VARIABLE)
  {
    if (m_datatype)
    {
      return m_datatype->getDatatypeSize();
    }
  }
  cerror("retreived dataType size on non variable node");
  return 0;
}

std::shared_ptr<datatype> node::getDatatype()
{
  if (m_node_type == NODE_TYPE_IDENTIFIER)
  {
    return m_declaration_node->getDatatype();
  }
  return m_datatype;
}

void node::addStatement(std::shared_ptr<node> statement)
{
  // stack size calculation moved to node class, and is performed after all tokens has been parsed!
  m_statements.push_back(statement);
  // TODO: add code for padding and allignemt?
  // x86 acquire minimum 16 byte stack size
}

void node::calculateStackOffset(int &stack_offset)
{
  if (m_node_type == NODE_TYPE_VARIABLE)
  {
    if (getIsFunctionArgument())
    {
      stack_offset += m_datatype->getDatatypeSize();
      // make the stack offset alligned to 4 byte
      stack_offset += datatype::Padding(m_datatype->getDatatypeSize(), DATA_SIZE_DWORD);
      setStackOffset(stack_offset);
      return;
    }
    else
    {
      stack_offset -= m_datatype->getDatatypeSize();
      // make the stack offset alligned to 4 byte
      m_padding = datatype::Padding(m_datatype->getDatatypeSize(), DATA_SIZE_DWORD);
      setStackOffset(stack_offset);
      stack_offset -= m_padding;
      return;
    }
  }
  else if (m_node_type == NODE_TYPE_BODY)
  {
    // we might have nested scopes {{}}, stack_offset can be something else that 0 from the previous scope
    // store for nested body size calculation, 0 if first scope in function
    int stack_offset_copy = stack_offset;
    setStackOffset(stack_offset);

    for (auto it = m_statements.begin(); it != m_statements.end(); ++it)
    {
      std::shared_ptr<node> statement_node = *it;
      statement_node->calculateStackOffset(stack_offset);
    }
    m_body_size = abs(stack_offset - stack_offset_copy);
    return;
  }
  else if (m_node_type == NODE_TYPE_FUNCTION)
  {
    // avoid adding the stack offset from the global variables
    int new_stack_offset = 0;
    // not forwared delcaration
    if (m_body_node)
    {
      m_body_node->calculateStackOffset(new_stack_offset);
      m_stack_size = m_body_node->getBodySize();
    }
    // positive stack offset for function arguments
    // first add stack addition, the size of ebp and esp that comes before the function arguments on the stack
    //  might be increased to 12 if the function returns a struct
    int argument_stack_offset = m_stack_addition;
    for (auto it = m_function_arguemnt.begin(); it != m_function_arguemnt.end(); ++it)
    {
      std::shared_ptr<node> argument_node = *it;
      argument_node->calculateStackOffset(argument_stack_offset);
      // not added to functions stack_size, are pushed to the stack by the caller before "call" instruction
    }
    return;
  }
  else if (m_node_type == NODE_TYPE_STATEMENT_IF)
  {
    m_body_node->calculateStackOffset(stack_offset);
    if (m_next_else_node)
    {
      m_next_else_node->calculateStackOffset(stack_offset);
    }
  }
  else if (m_node_type == NODE_TYPE_STATEMENT_ELSE)
  {
    m_body_node->calculateStackOffset(stack_offset);
  }
  else if (m_node_type == NODE_TYPE_STATEMENT_WHILE)
  {
    m_body_node->calculateStackOffset(stack_offset);
  }
  else if (m_node_type == NODE_TYPE_STATEMENT_FOR)
  {
    if (m_init_node)
    {
      // its not required to have a variable declaration in the init field. Therefore check!
      if (m_init_node->getNodeType() == NODE_TYPE_VARIABLE)
      {
        m_init_node->calculateStackOffset(stack_offset);
      }
    }
    m_body_node->calculateStackOffset(stack_offset);
  }
}

void node::generateExpressionFlag()
{
  if (STRINGS_EQUAL(m_string_value.c_str(), "+"))
  {
    m_exp_type = EXPRESSION_IS_ADDITION;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "-"))
  {
    m_exp_type = EXPRESSION_IS_SUBTRACTION;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "*"))
  {
    m_exp_type = EXPRESSION_IS_MULTPILICATION;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "/"))
  {
    m_exp_type = EXPRESSION_IS_DIVISION;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "%"))
  {
    m_exp_type = EXPRESSION_IS_MODULUS;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), ">"))
  {
    m_exp_type = EXPRESSION_IS_ABOVE;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "<"))
  {
    m_exp_type = EXPRESSION_IS_BELOW;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), ">="))
  {
    m_exp_type = EXPRESSION_IS_ABOVE_OR_EQUAL;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "<="))
  {
    m_exp_type = EXPRESSION_IS_BELOW_OR_EQUAL;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "!="))
  {
    m_exp_type = EXPRESSION_IS_NOT_EQUAL;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "=="))
  {
    m_exp_type = EXPRESSION_IS_EQUAL;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "&&"))
  {
    m_exp_type = EXPRESSION_LOGICAL_AND;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "||"))
  {
    m_exp_type = EXPRESSION_LOGICAL_OR;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "<<"))
  {
    m_exp_type = EXPRESSION_BITSHIFT_LEFT;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), ">>"))
  {
    m_exp_type = EXPRESSION_BITSHIFT_RIGHT;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "&"))
  {
    m_exp_type = EXPRESSION_IS_BITWISE_AND;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "|"))
  {
    m_exp_type = EXPRESSION_IS_BITWISE_OR;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "^"))
  {
    m_exp_type = EXPRESSION_IS_BITWISE_XOR;
  }
}
