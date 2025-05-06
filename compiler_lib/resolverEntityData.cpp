#include "pch.h"
#include "resolverEntityData.h"


resolverEntityData::resolverEntityData(int entity_type)
    :m_entity_type(entity_type),
    m_is_stack(false),
    offset(0)
{
}

void resolverEntityData::resolveVariableNode(std::shared_ptr<node> node)
{
    m_entity_type = ENTITY_TYPE_VARIABLE;
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
