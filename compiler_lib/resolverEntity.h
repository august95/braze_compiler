#pragma once
#include <string>
#include "resolverEntityData.h"
#include "node.h"

class resolverEntity
{
public:	
	resolverEntity();
	void createResolverEntityData();
	void addAddress(std::shared_ptr<node> node);
	void registerFunction(std::shared_ptr<node> node);
	std::string getAddress();
	void setResolverEntityData(std::shared_ptr < resolverEntityData > entity_data) { m_entity_data = entity_data; }
	std::shared_ptr < resolverEntityData > getResolverEntityData() { return m_entity_data; }
	std::shared_ptr < node > getNode() { return m_node; }

//private:
	int type;
	int flag;
	int offest;
	std::string m_name;
	std::shared_ptr < resolverEntityData > m_entity_data;
	std::shared_ptr < node > m_node;
	std::shared_ptr < datatype > m_datatype;
};

