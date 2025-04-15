#include "pch.h"
#include "resolver.h"

resolver::resolver()
{
}

void resolver::createNewScope()

{
  std::shared_ptr <resolverScope> new_scope = std::make_shared <resolverScope>();
  new_scope->init(m_next_scope);
  m_next_scope = new_scope;
}