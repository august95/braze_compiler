#include "../nodeStatement.h"

nodeStatement::nodeStatement()
{
}

nodeStatement::nodeStatement(nodeType node_type, filePosition file_position)
  :node(node_type, file_position)
{
}

void nodeStatement::calculateStackOffset(int& stack_offset)
{
  if (m_node_type == NODE_TYPE_STATEMENT_IF)
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