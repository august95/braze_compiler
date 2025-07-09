#include "pch.h"
#include "../resolverEntity.h"

resolverEntity::resolverEntity()
	:m_type(E_NONE),
	m_is_global(false),
 type(0),
 flag(0),
 offest(0)
{
}

resolverEntity::resolverEntity(std::shared_ptr<node> node)
	:m_type(E_NONE),
	m_node(node),
	m_is_global(false),
	type(0),
	flag(0),
	offest(0)
{
}

void resolverEntity::createResolverEntityData()
{
	m_entity_data = std::make_shared<resolverEntityData>();
}

void resolverEntity::addAddress(std::shared_ptr<node> node, bool is_local_stack)
{
	if(m_entity_data)
		m_entity_data->setVariableNode(node, is_local_stack);
}
void resolverEntity::registerFunction(std::shared_ptr<node> node)
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
