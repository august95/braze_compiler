#include "../nodeFunctionDeclaration.h"
#include "../node.h"

nodeFunctionDeclaration::nodeFunctionDeclaration()
:node(),
  m_stack_size(0),
  m_stack_addition(4),
  m_function_prototype(0)
{
  m_node_type = NODE_TYPE_FUNCTION_DECLARATION;
}

nodeFunctionDeclaration::nodeFunctionDeclaration(nodeType node_type, filePosition file_position)
  : node(node_type, file_position),
  m_stack_size(0),
  m_stack_addition(4),
  m_function_prototype(0)
{
}

void nodeFunctionDeclaration::calculateStackOffset(int& stack_offset)
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

