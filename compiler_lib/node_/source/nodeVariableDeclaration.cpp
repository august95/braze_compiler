#include "../nodeVariableDeclaration.h"
#include "../../braze_compiler.h"
#include "../node.h"

nodeVariableDeclaration::nodeVariableDeclaration()
  : m_is_function_argument(0),
  m_is_global(0),
  m_padding(0),
  m_stack_offset(0)
{
}

nodeVariableDeclaration::nodeVariableDeclaration(nodeType node_type, filePosition file_position)
  :node(node_type, file_position),
  m_is_function_argument(0),
  m_is_global(0),
  m_padding(0),
  m_stack_offset(0)
{
}

int nodeVariableDeclaration::getDatatypeSize()
{
  // local variables in the scope increment the stack size of that scope,
  // function parameters is not yet parsed by the parser. The parameters
  // will be located above the previous base pointer on the stack. Will
  // probably require an own stack size function

  if (m_datatype)
  {
    return m_datatype->getDatatypeSize();
  }
  return 0;
}

void nodeVariableDeclaration::calculateStackOffset(int& stack_offset)
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
}