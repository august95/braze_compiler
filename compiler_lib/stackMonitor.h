#pragma once
#include <vector>
#include <list>
#include "datatype.h"
#include <iostream>
#include <cstring>
#include <memory>

// todo, add more types
enum stackElementType
{
  UNKNOWN_ELEMENT,
  PUSHED_VAUE,
  BASE_POINTER
};

class stackMonitor
{
public:
  stackMonitor();
  void pushElement(stackElementType type, std::shared_ptr<datatype> datatype_, int offset_from_bp);
  void popElement(stackElementType type);
  std::shared_ptr<datatype> getDatatypeOnStack(int index = 0);
  int discardUnusedStack();
  void addStack(int stack_size);

  struct stackElement
  {
  public:
    stackElement(stackElementType type, std::shared_ptr<datatype> datatype, int offset_from_bp)
        : m_element_type(type), m_datatype(datatype), m_offset_from_bp(offset_from_bp)
    {
    }
    int m_offset_from_bp;
    //copy of the data type from the node tree, owned by the stackElement
    std::shared_ptr<datatype> m_datatype;
    stackElementType m_element_type;
  };

private:
  std::vector<std::shared_ptr<stackElement>> m_stack;
};
