#pragma once
#include "node.h"
#include "datatype.h"
#include <string>
#include <memory>

enum entityDataType
{
	NONE,
	FUNCTION,
	VARIABLE,
	STRUCT,
	SCOPE
};

class resolverEntityData
{
public:
	resolverEntityData(entityDataType entity_type = entityDataType::NONE);
	void setVariableNode(std::shared_ptr<node> node, bool is_local_stack = false);
	std::string getAddress() { return m_address; }
	std::string getBaseAddress() { return m_base_address; }
	void setStackAsmAddress(bool local_stack, std::string &address, int stack_offset);
	std::shared_ptr<node> getNode() { return m_node; }
	void registerFunction(std::shared_ptr<node> node);
	void setGlobalAsmAddress(std::string name, int offset = 0);

	// private:
	//  ie variable function or struct
	int m_entity_type;
	// This is the addresss [ebp-4][var_name+4]
	std::string m_address;
	// ebp, var_name
	std::string m_base_address;
	// -4
	int offset;
	int flags;

	bool m_is_stack;
	std::shared_ptr<datatype> m_datatype;
	std::shared_ptr<node> m_node;
};
