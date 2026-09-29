#pragma once
#include "../../node_/node.h"
#include "../../node_/nodeVariableDeclaration.h"
#include <string>
#include <memory>

class resolverEntityData
{
public:
  resolverEntityData();
  void setVariableNode(std::shared_ptr<nodeVariableDeclaration> node);
  std::string getAddress() const { return m_address; }
  void registerFunction(std::shared_ptr<node> node);
  void setGlobalAsmAddress(const std::string& name, int offset = 0);

private:
  std::string m_address;
};
