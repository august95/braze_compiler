#pragma once
#include "resolverScope.h"
#include "resolverEntity.h"
#include "resolverResult.h"
#include "node.h"
#include <vector>
#include <iostream>
#include <memory>
class resolver
{
public:
  resolver();
  void registerFunction(std::shared_ptr<node> function_node);
  void createNewScope(bool local_stack, bool stack);
  std::shared_ptr<resolverEntity> addEntity(std::shared_ptr<node> node, bool is_local_stack = false);
  void follow(std::shared_ptr<node> node, std::shared_ptr<resolverResult> &result);
  void removeScope();
  void initialize();

private:
  //  std::vector< std::shared_ptr < resolverScope> > m_scope;
  std::shared_ptr<resolverScope> m_root_scope;
  std::shared_ptr<resolverScope> m_current_scope;
};
