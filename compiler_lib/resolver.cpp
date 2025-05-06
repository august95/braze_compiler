#include "pch.h"
#include "resolver.h"
#include "braze_compiler.h"

resolver::resolver()
{
}

void resolver::registerFunction(std::shared_ptr<node> function_node)
{ 
  std::shared_ptr< resolverEntity> data = std::make_shared<resolverEntity>();
  std::shared_ptr< resolverScope> scope = std::make_shared<resolverScope>();
  scope->addScopeData(data);
  m_scope.push_back(scope);
}

void resolver::createNewScope(bool local_stack, bool stack)
{
  std::shared_ptr< resolverScope> scope = std::make_shared<resolverScope>();
  scope->setLocalStack(local_stack);
  m_scope.push_back(scope);
}

void resolver::addEntity(std::shared_ptr<node> node)
{
  if (node->getNodeType() == NODE_TYPE_VARIABLE)
  {
    std::shared_ptr<resolverEntity> entity = std::make_shared<resolverEntity>();
    entity->createResolverEntityData();
    entity->addAddress(node);
    m_scope.back()->addScopeData(entity);
    //std::cout << "resolver: added variable " << node->getStringValue()<< "  add addess: " << entity->getAddress() << std::endl;
  }
}

std::shared_ptr<resolverEntity> resolver::follow(std::shared_ptr<node> node)
{
  for (auto it = m_scope.rbegin(); it != m_scope.rend(); ++it)
  {
    std::shared_ptr<resolverEntity> entity = (*it)->follow(node);
    if (entity)
      return entity;
  }
}

void resolver::removeScope()
{
  if (m_scope.empty())
  {
    cerror("tried to remove root scope!");
    assert(0);
  }
  m_scope.pop_back();
}

void resolver::initialize()
{
  std::shared_ptr< resolverScope> scope = std::make_shared<resolverScope>();
  scope->setRootScope(true);
  m_scope.push_back(scope);
}
