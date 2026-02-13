#include "../nodeBody.h"
#include "../node.h"

nodeBody::nodeBody()
  : node(),
  m_body_size(0),
  m_stack_offset(0)
{
}

nodeBody::nodeBody(nodeType node_type, filePosition file_position)
  : node(node_type, file_position),
  m_body_size(0),
  m_stack_offset(0)
{
}

void nodeBody::calculateStackOffset(int& stack_offset)
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

void nodeBody::addStatement(std::shared_ptr<node> statement)
{
  // stack size calculation moved to node class, and is performed after all tokens has been parsed!
  m_statements.push_back(statement);
  // TODO: add code for padding and allignemt?
  // x86 acquire minimum 16 byte stack size
}
