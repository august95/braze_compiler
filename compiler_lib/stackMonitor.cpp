#include "pch.h"
#include "stackMonitor.h"
#include <assert.h>

stackMonitor::stackMonitor()
{
}

void stackMonitor::pushElement(stackElementType type)
{
  m_stack.push_back(std::make_shared< stackElement >(type));
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

