#pragma once
#include "resolverScope.h"
#include "resolverEntity.h"
#include "resolverResult.h"
#include "../../node_/node.h"
#include "../../node_/nodeFunctionDeclaration.h"
#include <memory>

class resolver
{
public:
  resolver();
  void registerFunction(std::shared_ptr<nodeFunctionDeclaration> function_node);
  std::shared_ptr<resolverEntity> registerVariable(std::shared_ptr<node> node);
  void createNewScope();
  std::shared_ptr<resolverResult> follow(std::shared_ptr<node> node);
  void removeScope();
  void initialize();

private:
  std::shared_ptr<resolverScope> m_root_scope;
  std::shared_ptr<resolverScope> m_current_scope;
};
