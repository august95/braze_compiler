#include "../../../source/pch.h"
#include "../../../braze_compiler.h"
#include "../resolverEntityData.h"

resolverEntityData::resolverEntityData(entityDataType entity_type)
    : m_entity_type(entity_type),
      m_is_stack(false),
      offset(0),
      m_ptr_depth(0)
{
}

void resolverEntityData::setVariableNode(std::shared_ptr<nodeVariableDeclaration> node)
{
  m_entity_type = VARIABLE;
  m_node = node;
  m_datatype = node->getDatatype();
  m_is_stack = true;

  if (node->getIsGlobal())
  {
    setGlobalAsmAddress(m_node->getStringValue(), node->getIsGlobal() ? 0 : node->getStackOffset());
  }
  else
  {
    setStackAsmAddress(!node->getIsGlobal(), m_address, node->getStackOffset());
  }
}

void resolverEntityData::setStackAsmAddress(bool local_stack, std::string &address, int stack_offset)
{
  if (local_stack)
  {
    if (stack_offset > 0)
    {
      address = "ebp+" + std::to_string(stack_offset);
      return;
    }
    address = "ebp" + std::to_string(stack_offset);
  }
  else
  {
    address = "variable name";
  }
}
void resolverEntityData::registerFunction(std::shared_ptr<node> node)
{
  m_entity_type = FUNCTION;
  setGlobalAsmAddress(node->getStringValue(), 0);
}

void resolverEntityData::setGlobalAsmAddress(std::string name, int offset)
{
  if (offset == 0)
  {
    m_address += name;
    return;
  }
  m_address += name + "+" + std::to_string(offset);
}
