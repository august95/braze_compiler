#include "pch.h"
#include "nodeExpresssion.h"

nodeExpresssion::nodeExpresssion()
	:node(nodeType::NODE_TYPE_UNDEFINED)

{

}

nodeExpresssion::nodeExpresssion(nodeType node_type, filePosition file_position)
	:node(node_type, file_position),
	m_number_val(0)
{

}

nodeExpresssion::nodeExpresssion(nodeType node_type, filePosition file_position, std::string string_value)
	:node(node_type, file_position),
	m_string_value(string_value),
	m_number_val(0)
{

}

nodeExpresssion::nodeExpresssion(nodeType node_type, filePosition file_position, unsigned long number_value)
	:node(node_type, file_position),
	m_number_val(number_value)
{

}

nodeExpresssion::nodeExpresssion(nodeType node_type, filePosition file_position, std::shared_ptr < node > left_node, std::shared_ptr < node > right_node)
	:node(node_type, file_position),
	m_number_val(0),
	m_left_node(left_node),
	m_right_node(right_node)
{

}
