#pragma once
#include <vector>
#include "datatype.h"
#include <iostream>

//todo, add more types
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
  void pushElement(stackElementType type);
  void popElement(stackElementType type);

  class stackElement
  {
  public:

    stackElement(stackElementType type)
      :m_element_type(type)
    { }

    std::shared_ptr< datatype > m_datatype;
    stackElementType m_element_type;
  };
private:


  std::vector < std::shared_ptr < stackElement > > m_stack;
};

