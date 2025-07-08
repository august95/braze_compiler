#include "pch.h"
#include "../stackMonitor.h"
#include <assert.h>

stackMonitor::stackMonitor()
{
}

void stackMonitor::pushElement(stackElementType type, std::shared_ptr < datatype > datatype, int offset_from_bp)
{
  m_stack.push_back(std::make_shared< stackElement >(type, datatype, offset_from_bp));
}

void stackMonitor::popElement(stackElementType type)
{
  std::shared_ptr < stackElement > stack_element = m_stack.back();
  m_stack.pop_back();
  if (type != stack_element->m_element_type)
  {
    assert(0);
  }
}

std::shared_ptr<datatype> stackMonitor::getDatatypeOnStack(int index)
{
  int i = 0;
  for (auto it = m_stack.rbegin(); it != m_stack.rend(); ++it) {
    if (i == index)
    {
      return (*it)->m_datatype;
    }
    i++;
  }
  return std::shared_ptr<datatype>();
}

