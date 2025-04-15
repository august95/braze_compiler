#pragma once
#include "resolverScope.h"
#include <iostream>
class resolver
{
public:
  resolver();

  void createNewScope();
private:
  std::shared_ptr <resolverScope> m_next_scope;
  std::shared_ptr <resolverScope> m_next_current;

};

