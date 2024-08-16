#pragma once
#include "node.h"
#include "filePosition.h"
#include <iostream>

class nodeExpresssion :
    public node
{
public:
    nodeExpresssion();
    nodeExpresssion(nodeType node_type, filePosition file_position);
    nodeExpresssion(nodeType node_type, filePosition file_position, std::string string_value);
    nodeExpresssion(nodeType node_type, filePosition file_position, unsigned long number_value);
    nodeExpresssion(nodeType node_type, filePosition file_position, std::shared_ptr < node > left_node, std::shared_ptr < node > right_node);

protected:

    std::shared_ptr < node > m_left_node;
    std::shared_ptr < node > m_right_node;

	std::string m_string_value;
	unsigned long m_number_val;

};

