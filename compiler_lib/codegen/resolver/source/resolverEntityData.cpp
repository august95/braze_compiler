#include "../../../source/pch.h"
#include "../../../braze_compiler.h"
#include "../resolverEntityData.h"

resolverEntityData::resolverEntityData()
{
}

void resolverEntityData::setVariableNode(std::shared_ptr<nodeVariableDeclaration> node)
{
  if (node->getIsGlobal())
  {
    setGlobalAsmAddress(node->getStringValue());
  }
  else
  {
    const int stack_offset = node->getStackOffset();
    m_address = stack_offset > 0
        ? "ebp+" + std::to_string(stack_offset)
        : "ebp" + std::to_string(stack_offset);
  }
}

void resolverEntityData::registerFunction(std::shared_ptr<node> node)
{
  setGlobalAsmAddress(node->getStringValue());
}

void resolverEntityData::setGlobalAsmAddress(const std::string& name, int offset)
{
  m_address = name;
  if (offset != 0)
  {
    m_address += "+" + std::to_string(offset);
  }
}
