#include "../../../source/pch.h"
#include "../../../braze_compiler.h"
#include "../resolverEntity.h"
#include "../../../node_/nodeVariableDeclaration.h"

resolverEntity::resolverEntity()
  : m_type(E_NONE),
    m_is_global(false),
    type(0),
    flag(0),
    offest(0),
    m_function_call_stack_size(0),
    m_code_gen_instruction(0),
    m_indirection_depth(0)
{
}

resolverEntity::resolverEntity(std::shared_ptr<node> node)
  : m_type(E_NONE),
    m_node(node),
    m_is_global(false),
    type(0),
    flag(0),
    offest(0),
    m_function_call_stack_size(0),
    m_code_gen_instruction(0),
    m_indirection_depth(0)
{
}

void resolverEntity::createResolverEntityData()
{
  m_entity_data = std::make_shared<resolverEntityData>();
}

void resolverEntity::addAddress(std::shared_ptr<node> node)
{
  if (m_entity_data)
    m_entity_data->setVariableNode(cast_node<nodeVariableDeclaration>(node));
}
void resolverEntity::registerFunction(std::shared_ptr<nodeFunctionDeclaration> node)
{
  if (m_entity_data)
    m_entity_data->registerFunction(node);
  m_type = E_FUNCTION;
  m_name = node->getStringValue();
  m_datatype = node->getReturnDatatype();
}

std::string resolverEntity::getAddress()
{
  if (m_entity_data)
    return m_entity_data->getAddress();
  return "";
}
