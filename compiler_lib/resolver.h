#pragma once
#include "resolverScope.h"
#include "resolverEntity.h"
#include "resolverResult.h"
#include "node.h"
#include <vector>
#include <iostream>
class resolver
{
public:
  resolver();
  void registerFunction( std::shared_ptr < node > function_node);
  void createNewScope(bool local_stack, bool stack);
  std::shared_ptr<resolverEntity> addEntity(std::shared_ptr<node> node, bool is_local_stack=false);
  std::shared_ptr<resolverResult> follow(std::shared_ptr<node> node, std::shared_ptr<resolverResult>& result);
  void removeScope();
  void initialize();
private:
//  std::vector< std::shared_ptr < resolverScope> > m_scope;
  std::shared_ptr < resolverScope> m_root_scope;
  std::shared_ptr < resolverScope> m_current_scope;

};

