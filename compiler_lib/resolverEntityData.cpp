#include "pch.h"
#include "resolverEntityData.h"


resolverEntityData::resolverEntityData(entityType entity_type)
    :m_entity_type(entity_type),
    m_is_stack(false),
    offset(0)
{
}

void resolverEntityData::resolveVariableNode(std::shared_ptr<node> node)
{
    m_entity_type = VARIABLE;
    m_node = node;
    m_datatype = node->getDatatype();
    m_is_stack = true;

    //should this be a function?
    //needs to handle global variables as well, not only offset from local base pointer
    calculateAddress(true, m_address, node->getStackOffset());
}

void resolverEntityData::calculateAddress(bool local_stack, std::string& address, int stack_offset)
{
    if (local_stack)
    {
        address = "ebp -" + std::to_string(stack_offset);
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
