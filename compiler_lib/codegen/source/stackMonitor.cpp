#include "../../source/pch.h"
#include "../stackMonitor.h"
#include <assert.h>
#include "../../braze_compiler.h"

stackMonitor::stackMonitor()
{
}

void stackMonitor::pushElement(stackElementType type, std::shared_ptr<datatype> datatype_, int offset_from_bp)
{
  std::shared_ptr< datatype > datatype_copy = std::make_shared <datatype>();
  if (datatype_)
  {
    *datatype_copy = *datatype_;
  }

  m_stack.push_back(std::make_shared<stackElement>(type, datatype_, offset_from_bp));
}

void stackMonitor::popElement(stackElementType type)
{
  std::shared_ptr<stackElement> stack_element = m_stack.back();
  m_stack.pop_back();
  if (type != stack_element->m_element_type)
  {
    assert(0);
  }
}

std::shared_ptr<datatype> stackMonitor::getDatatypeOnStack(int index)
{
  int i = 0;
  for (auto it = m_stack.rbegin(); it != m_stack.rend(); ++it)
  {
    if (i == index)
    {
      return (*it)->m_datatype;
    }
    i++;
  }
  return std::shared_ptr<datatype>();
}

int stackMonitor::discardUnusedStack()
{
  int stack_addition = 0;
  for (auto it = m_stack.rbegin(); it != m_stack.rend();)
  {
    std::shared_ptr<stackElement> element = (*it);
    if (element->m_element_type != BASE_POINTER)
    {
      stack_addition += DATA_SIZE_DWORD;
      m_stack.pop_back();
      it = m_stack.rbegin();
      // fixme: add custom datatsize
    }
    else
    {
      return stack_addition;
    }
  }
}

void stackMonitor::addStack(int stack_size)
{
  int stack_added = 0;
  for (auto it = m_stack.rbegin(); it != m_stack.rend();)
  {
    std::shared_ptr<stackElement> element = (*it);
    if (element->m_element_type != BASE_POINTER)
    {
      stack_added += DATA_SIZE_DWORD;
      m_stack.pop_back();
      it = m_stack.rbegin();
      if (stack_added == stack_size)
      {
        return;
      }
      // fixme: add custom datatsize
    }
    else
    {
      cerror("invalid stack after function call clean up!!");
      return;
      // assert(0);
    }
  }
}
