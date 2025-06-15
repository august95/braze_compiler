#include "pch.h"
#include "braze_compiler.h"
#include "resolver.h"

resolver::resolver()
{
}

void resolver::registerFunction(std::shared_ptr<node> function_node)
{ 
  std::shared_ptr< resolverEntity> data = std::make_shared<resolverEntity>();
  data->createResolverEntityData();
  data->registerFunction(function_node);
  m_root_scope->addScopeEntity(data);
}

void resolver::createNewScope(bool local_stack, bool stack)
{
  std::shared_ptr< resolverScope> scope = std::make_shared<resolverScope>();
  m_current_scope->setNextScope(scope);
  scope->setPrevScope(m_current_scope);
  scope->setLocalStack(local_stack);
  //todo: add flags to scope data?
  m_current_scope = scope;
}

std::shared_ptr<resolverEntity> resolver::addEntity(std::shared_ptr<node> node)
{
  std::shared_ptr<resolverEntity> entity = std::make_shared<resolverEntity>();
  if (node->getNodeType() == NODE_TYPE_VARIABLE)
  {
    entity->createResolverEntityData();
    entity->addAddress(node);
    m_current_scope->addScopeEntity(entity);
    //std::cout << "resolver: added variable " << node->getStringValue()<< "  add addess: " << entity->getAddress() << std::endl;
  }
  return entity;
}

std::shared_ptr<resolverEntity> resolver::follow(std::shared_ptr<node> node)
{
  return  m_current_scope->follow(node);
}

void resolver::removeScope()
{
  m_current_scope = m_current_scope->getPrevScope();
}

void resolver::initialize()
{
  m_root_scope = std::make_shared<resolverScope>();
  m_root_scope->setRootScope(true);
  m_current_scope = m_root_scope;
}
