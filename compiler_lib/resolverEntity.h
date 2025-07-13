#pragma once
#include <string>
#include <list>
#include "resolverEntityData.h"
//#include "resolverscope.h"
#include "node.h"
#include <memory>

enum entityType
{
	E_NONE,
	E_FUNCTION,
	E_VARIABLE,
	E_FUNCTION_CALL,
	E_POINTER,
	E_ARRAY
};	
class resolverEntity
{
public:	
	resolverEntity();
	resolverEntity(std::shared_ptr < node > node);
	void createResolverEntityData();
	void addAddress(std::shared_ptr<node> node, bool is_stack=false);
	void registerFunction(std::shared_ptr<node> node);
	std::string getAddress();
	void setResolverEntityData(std::shared_ptr < resolverEntityData > entity_data) { m_entity_data = entity_data; }
	std::shared_ptr < resolverEntityData > getResolverEntityData() { return m_entity_data; }
	std::shared_ptr < node > getNode() { return m_node; }
	void setEntityType(entityType entity_type) { m_type = entity_type; }
	entityType getEntityType() { return m_type;  }
	void setGlobalEntity(bool global_entity) { m_is_global = global_entity; }
	//void setScope(std::shared_ptr < resolverScope> scope) { m_scope = scope; }
	std::shared_ptr < datatype > getDatatype() { return m_datatype; }
	void setDatatype(std::shared_ptr < datatype > datatype) { m_datatype = datatype; }
	void addFunctionArgumnet(std::shared_ptr < node > node_) { m_function_arguments.push_back(node_); }
	std::list < std::shared_ptr < node > > getFunctionArguments() { return m_function_arguments; }
	void setFunctionCallStacksize(int stack_size) { m_function_call_stack_size = stack_size; }
	int getFunctionCallStacksize() { return m_function_call_stack_size; }
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

	//function calls
	int m_function_call_stack_size;
	std::list < std::shared_ptr < node > > m_function_arguments;

};

