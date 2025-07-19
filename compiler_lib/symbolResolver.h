#pragma once
#include "scope.h"
#include <memory>

class symbolResolver
{
public:
  symbolResolver();
  void finishScope();
  void addNodeToCurrentScope(std::shared_ptr<node> node);
  void newScope();
  std::shared_ptr<node> findDeclerationNode(std::shared_ptr<node> identifier);

private:
  std::shared_ptr<node> findVariableDeclerationNode(std::shared_ptr<node> identifier);
  std::shared_ptr<node> findFunctionNode(std::shared_ptr<node> identifier);

private:
  std::shared_ptr<scope> m_root_scope;
  std::shared_ptr<scope> m_last_scope;
};
