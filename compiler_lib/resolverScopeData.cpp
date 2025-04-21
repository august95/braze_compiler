#include "pch.h"
#include "resolverScopeData.h"

resolverEntity::resolverEntity()
  :m_entity_type(ENTITY_TYPE_NONE)
{
}

void resolverEntity::resolveVariableNode(std::shared_ptr<node> node)
{
  m_entity_type = ENTITY_TYPE_VARIABLE;
  m_node = node;
  m_datatype = node->getDatatype(); 
  m_is_stack = true;

  //should this be a function?
  //needs to handle global variables as well, not only offset from local base pointer
  calculateAddress(true , m_address, node->getStackOffset());

}

void resolverEntity::calculateAddress(bool local_stack, std::string& address, int stack_offset)
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
