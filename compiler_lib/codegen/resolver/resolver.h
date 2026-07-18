#pragma once
#include "resolverScope.h"
#include "resolverEntity.h"
#include "resolverResult.h"
#include "../../node_/node.h"
#include "../../node_/nodeFunctionDeclaration.h"
#include <vector>
#include <iostream>
#include <memory>
class resolver
{
public:
  resolver();
  void registerFunction(std::shared_ptr<nodeFunctionDeclaration> function_node);
  std::shared_ptr<resolverEntity> registerVariable(std::shared_ptr<node> node);
  void createNewScope(bool local_stack, bool stack);
  void follow(std::shared_ptr<node> node, std::shared_ptr<resolverResult> &result);
  void removeScope();
  void initialize();

private:
  //  std::vector< std::shared_ptr < resolverScope> > m_scope;
  std::shared_ptr<resolverScope> m_root_scope;
  std::shared_ptr<resolverScope> m_current_scope;
};
