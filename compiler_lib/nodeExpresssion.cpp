#include "pch.h"
#include "nodeExpresssion.h"
#include "ExpressionableOperatorPrecedence.h"

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

nodeExpresssion::nodeExpresssion(nodeType node_type, filePosition file_position, std::shared_ptr < nodeExpresssion > left_node, std::shared_ptr < nodeExpresssion > right_node)
	:node(node_type, file_position),
	m_number_val(0),
	m_left_node(left_node),
	m_right_node(right_node)
{

}

/*
changing order according to precedence:
    *
50     +
	30   20

needs to be
		+
	*		20
50	30

after lexing,in a longer expression, if a nodes operator '*' has priority (* has priority over +)
over right child's operator '+', that part of the node tree will be reordered. We want the operators
with the lowest priority closer to the root of the tree. That will make the code generation easier.
For example getting the '=' at last, after the value 50*30+20 has been calcuclated. 

Another case:
	+
50     *
	30   20

will stay like:
	+
50     *
	30   20

*/

void nodeExpresssion::reorderExpression()
{
	if (m_node_type != nodeType::NODE_TYPE_EXPRESSION)
	{
		//might be number, identifier or string. We need an operator
		return;
	}

	if (getLeftNode()->getNodeType() != nodeType::NODE_TYPE_EXPRESSION &&
		getRightNode() && getRightNode()->getNodeType() != nodeType::NODE_TYPE_EXPRESSION)
	{
		return;
	}

	if (getLeftNode()->getNodeType() != nodeType::NODE_TYPE_EXPRESSION &&
		getRightNode() && getRightNode()->getNodeType() == nodeType::NODE_TYPE_EXPRESSION)
	{
		//operator on the right side
		if (precedence::leftOperatorHasPriority(m_string_value, getRightNode()->getStringValue()))
		{
			shiftChildrenLeft();
		}
	}


}

void nodeExpresssion::shiftChildrenLeft()
{
	std::string right_operator = getRightNode()->getStringValue();
	std::shared_ptr < nodeExpresssion > new_left_exp_node = getLeftNode();
	std::shared_ptr < nodeExpresssion > new_right_exp_node = getRightNode()->getLeftNode();

	std::shared_ptr < nodeExpresssion > new_left_operand = std::make_shared< nodeExpresssion >(nodeType::NODE_TYPE_EXPRESSION, getFilePosition(), new_left_exp_node,  new_right_exp_node);
	new_left_operand->setStringValue(m_string_value);
	std::shared_ptr < nodeExpresssion > new_right_operand = getRightNode()->getRightNode();
	setLeftNode(new_left_operand);
	setRightNode(new_right_operand);
	setStringValue(right_operator);
}
