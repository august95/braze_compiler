#include "pch.h"
#include "resolverEntity.h"

resolverEntity::resolverEntity()
{
}

void resolverEntity::createResolverEntityData()
{
	m_entity_data = std::make_shared<resolverEntityData>();
}

void resolverEntity::addAddress(std::shared_ptr<node> node)
{
	m_node = node;
	if(m_entity_data)
		m_entity_data->resolveVariableNode(node);
}
void resolverEntity::registerFunction(std::shared_ptr<node> node)
{
	if (m_entity_data)
		m_entity_data->registerFunction(node);

	m_name = node->getStringValue();
	m_node = node;
	m_datatype = node->getReturnDatatype();
}

std::string resolverEntity::getAddress()
{
	if (m_entity_data)
		return m_entity_data->getAddress();
	return "";
}
