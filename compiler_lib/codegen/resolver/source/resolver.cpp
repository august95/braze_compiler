#include "../../../source/pch.h"
#include "../../../braze_compiler.h"
#include "../resolver.h"

resolver::resolver()
{
}

void resolver::registerFunction(std::shared_ptr<nodeFunctionDeclaration> function_node)
{
  std::shared_ptr<resolverEntity> resolver_entity = std::make_shared<resolverEntity>(function_node);
  resolver_entity->createResolverEntityData();
  resolver_entity->registerFunction(function_node);
  m_root_scope->addScopeEntity(resolver_entity);
}

std::shared_ptr<resolverEntity> resolver::registerVariable(std::shared_ptr<node> node)
{
  std::shared_ptr<resolverEntity> entity = std::make_shared<resolverEntity>(node);
  if (node->getNodeType() == NODE_TYPE_VARIABLE)
  {
    entity->createResolverEntityData();
    entity->addAddress(node);
    m_current_scope->addScopeEntity(entity);
    entity->setEntityType(E_VARIABLE);
  }
  return entity;
}

void resolver::createNewScope()
{
  m_current_scope = std::make_shared<resolverScope>(m_current_scope);
}

std::shared_ptr<resolverResult> resolver::follow(std::shared_ptr<node> node)
{
  std::shared_ptr<resolverResult> result = std::make_shared<resolverResult>();
  m_current_scope->follow(node, result);
  return result;
}

void resolver::removeScope()
{
  if (m_current_scope == m_root_scope)
  {
    cerror("tried to remove root scope!");
    assert(false);
    return;
  }
  m_current_scope = m_current_scope->getParent();
}

void resolver::initialize()
{
  m_root_scope = std::make_shared<resolverScope>();
  m_current_scope = m_root_scope;
}
