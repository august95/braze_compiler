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
    nodeExpresssion(nodeType node_type, filePosition file_position, std::shared_ptr < nodeExpresssion > left_node, std::shared_ptr < nodeExpresssion > right_node);

    void setStringValue(std::string string_value) { m_string_value = string_value; }
    std::shared_ptr < nodeExpresssion > getLeftNode() { return m_left_node; }
    std::shared_ptr < nodeExpresssion > getRightNode() { return m_right_node; }
    std::string getStringValue() { return m_string_value; }
    void reorderExpression();

protected:
    void setRightNode(std::shared_ptr < nodeExpresssion > right_node) { m_right_node = right_node; }
    void setLeftNode(std::shared_ptr < nodeExpresssion > left_node) { m_left_node = left_node; }
    void shiftChildrenLeft();     
    std::shared_ptr < nodeExpresssion > m_left_node;
    std::shared_ptr < nodeExpresssion > m_right_node;

	std::string m_string_value;
	unsigned long m_number_val;

};

