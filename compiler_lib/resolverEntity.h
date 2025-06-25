#pragma once
#include <string>
#include "resolverEntityData.h"
//#include "resolverscope.h"
#include "node.h"

enum entityType
{
	E_NONE,
	E_FUNCTION
};	
class resolverEntity
{
public:	
	resolverEntity();
	resolverEntity(std::shared_ptr < node > node);
	void createResolverEntityData();
	void addAddress(std::shared_ptr<node> node, bool is_stack=false, bool is_global=false);
	void registerFunction(std::shared_ptr<node> node);
	std::string getAddress();
	void setResolverEntityData(std::shared_ptr < resolverEntityData > entity_data) { m_entity_data = entity_data; }
	std::shared_ptr < resolverEntityData > getResolverEntityData() { return m_entity_data; }
	std::shared_ptr < node > getNode() { return m_node; }
	void setEntityType(entityType entity_type) { m_type = entity_type; }
	void setGlobalEntity(bool global_entity) { m_is_global = global_entity; }
	//void setScope(std::shared_ptr < resolverScope> scope) { m_scope = scope; }

//private:
	int type;
	int flag;
	int offest;
	bool m_is_global;
	std::string m_name;
	//std::shared_ptr < resolverScope> m_scope;
	entityType m_type;
	std::shared_ptr < resolverEntityData > m_entity_data;
	std::shared_ptr < node > m_node;
	std::shared_ptr < datatype > m_datatype;
};

