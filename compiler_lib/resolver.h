#pragma once
#include "resolverScope.h"
#include "resolverScopeData.h"
#include "node.h"
#include <vector>
#include <iostream>
class resolver
{
public:
  resolver();
  void registerFunction( std::shared_ptr < node > function_node);
  void createNewScope(bool local_stack, bool stack);
  void addEntity(std::shared_ptr<node> node);
  std::shared_ptr<resolverEntity> follow(std::shared_ptr<node> node);
  void removeScope();
  void initialize();
private:
  std::vector< std::shared_ptr < resolverScope> > m_scope;

};

