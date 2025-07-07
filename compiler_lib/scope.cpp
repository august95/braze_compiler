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

std::shared_ptr<node> scope::find(std::shared_ptr<node> identifier)
{
  for (auto nodes : m_scope_nodes)
  {
    if (STRINGS_EQUAL(nodes->getStringValue().c_str(), identifier->getStringValue().c_str()))
    {
      return nodes;
    }
  }
  if (!m_is_root_scope)
  {
    return m_parent->find(identifier);
  }
  return std::shared_ptr<node>();


}
