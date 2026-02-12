#include "../../source/pch.h"
#include "../../braze_compiler.h"
#include "../resolver.h"

resolver::resolver()
{
}

void resolver::registerFunction(std::shared_ptr<nodeFunctionDeclaration> function_node)
{
  std::shared_ptr<resolverEntity> resolver_entity = std::make_shared<resolverEntity>(function_node);
  resolver_entity->createResolverEntityData();
  resolver_entity->registerFunction(function_node);
  //  resolver_entity->setScope(m_current_scope);
  m_root_scope->addScopeEntity(resolver_entity);
}

void resolver::createNewScope(bool local_stack, bool stack)
{
  std::shared_ptr<resolverScope> scope = std::make_shared<resolverScope>();
  m_current_scope->setNextScope(scope);
  scope->setPrevScope(m_current_scope);
  scope->setLocalStack(local_stack);
  scope->setStack(stack);
  // todo: add flags to scope data?
  m_current_scope = scope;
}

std::shared_ptr<resolverEntity> resolver::addEntity(std::shared_ptr<node> node, bool is_local_stack)
{
  std::shared_ptr<resolverEntity> entity = std::make_shared<resolverEntity>(node);
  if (node->getNodeType() == NODE_TYPE_VARIABLE)
  {
    entity->createResolverEntityData();
    entity->addAddress(node, is_local_stack);
    m_current_scope->addScopeEntity(entity);
    entity->setEntityType(E_VARIABLE);
    // std::cout << "resolver: added variable " << node->getStringValue()<< "  add addess: " << entity->getAddress() << std::endl;
  }
  return entity;
}

void resolver::follow(std::shared_ptr<node> node, std::shared_ptr<resolverResult> &result)
{
  result = std::make_shared<resolverResult>();
  m_current_scope->follow(node, result);
}

void resolver::removeScope()
{
  m_current_scope = m_current_scope->getPrevScope();
  // todo add callback on deletion of scope
}

void resolver::initialize()
{
  m_root_scope = std::make_shared<resolverScope>();
  m_root_scope->setRootScope(true);
  m_current_scope = m_root_scope;
}
