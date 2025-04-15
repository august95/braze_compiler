#include "pch.h"
#include "node.h"


node::node()
	:m_body_size(0),
	m_node_type(nodeType::NODE_TYPE_BLANK),
	m_number_val(0),
	m_stack_offset(0)
{

}

node::node(filePosition file_position)
	:m_file_position(file_position),
	m_node_type(nodeType::NODE_TYPE_BLANK),
	m_body_size(0),
	m_number_val(0),
	m_stack_offset(0)
{
}

node::node(nodeType node_type, filePosition file_position)
	:m_node_type(node_type),
	m_file_position(file_position),
	m_body_size(0),
	m_number_val(0),
	m_stack_offset(0)
{

}


bool node::isValidExpressionType()
{
	return	m_node_type == NODE_TYPE_EXPRESSION ||
		m_node_type == NODE_TYPE_EXPRESSION_PARANTHESES ||
		m_node_type == NODE_TYPE_IDENTIFIER ||
		m_node_type == NODE_TYPE_UNARY ||
		m_node_type == NODE_TYPE_NUMBER ||
		m_node_type == NODE_TYPE_STRING;
}

int node::getDatatypeSize()
{
	//local variables in the scope increment the stack size of that scope,
	//function parameters is not yet parsed by the parser. The parameters
	//will be located above the previous base pointer on the stack. Will 
	//probably require an own stack size function
	if (m_node_type == NODE_TYPE_VARIABLE)
	{
		if (m_datatype)
		{
			return m_datatype->getDatatypeSize();
		}
	}

	else if (m_node_type == NODE_TYPE_BODY)
	{
		return m_body_size;
	}

	return 0;
}

void node::addStatement(std::shared_ptr<node> statement, int stack_offset)
{
	m_statements.push_back(statement);
	m_body_size += statement->getDatatypeSize();
	if (m_node_type == NODE_TYPE_VARIABLE || m_node_type == NODE_TYPE_BODY) //nested scopes
	{
		statement->setStackOffset(stack_offset);
	}
	//TODO: add code for padding and allignemt?
	//x86 acquire minimum 16 byte stack size
}
