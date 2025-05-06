#pragma once
#include "node.h"
#include "datatype.h"
#include <string>

enum
{
	ENTITY_TYPE_NONE,
	ENTITY_TYPE_FUNCTION,
	ENTITY_TYPE_VARIABLE,
	ENTITY_TYPE_STRUCT
};

class resolverEntityData
{
public:
	resolverEntityData();
	void resolveVariableNode(std::shared_ptr<node> node);
	std::string getAddress() { return m_address; }
	std::string getBaseAddress() { return m_base_address; }
	void calculateAddress(bool local_stack, std::string& address, int stack_offset);
	std::shared_ptr < node > getNode() { return m_node; }


private:
	// ie variable function or struct
	int m_entity_type;
	// This is the addresss [ebp-4][var_name+4]
	std::string m_address;
	// ebp, var_name
	std::string m_base_address;
	// -4
	int offset;

	bool m_is_stack;
	std::shared_ptr < datatype> m_datatype;
	std::shared_ptr < node > m_node;

};


