#include "pch.h"
#include "scope.h"
#include "braze_compiler.h"

scope::scope()
  :m_size(0),
  m_flags(0),
  m_is_root_scope(false)
{

}

void scope::init(std::shared_ptr<scope> last_scope, bool is_root_scope)
{
  m_is_root_scope = is_root_scope;

  if (is_root_scope)
  {
    return;
  }
  m_parent = last_scope;
}

void scope::addNode(std::shared_ptr<node> node)
{
  m_scope_nodes.push_back(node);
}
