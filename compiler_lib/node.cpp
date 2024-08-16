#include "pch.h"
#include "node.h"

node::node()
{

}

node::node(nodeType node_type)
	:m_node_type(node_type)
{

}

node::node(nodeType node_type, filePosition file_position)
	:m_node_type(node_type),
	m_file_position(file_position)
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

