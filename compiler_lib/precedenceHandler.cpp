#include "pch.h"
#include "precedenceHandler.h"
#include "ExpressionableOperatorPrecedence.h"

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


void precedenceHandler::shiftChildrenLeft(std::shared_ptr < node > node_)
{
	std::string right_operator = node_->getRightNode()->getStringValue();
	std::shared_ptr < node > new_left_exp_node = node_->getLeftNode();
	std::shared_ptr < node > new_right_exp_node = node_->getRightNode()->getLeftNode();

	std::shared_ptr < node > new_left_operand = std::make_shared< node >(nodeType::NODE_TYPE_EXPRESSION, node_->getFilePosition());
	new_left_operand->setLeftNode(new_left_exp_node);
	new_left_operand->setRightNode(new_right_exp_node);
	new_left_operand->setStringValue(node_->getStringValue());
	std::shared_ptr < node > new_right_operand = node_->getRightNode()->getRightNode();
	node_->setLeftNode(new_left_operand);
	node_->setRightNode(new_right_operand);
	node_->setStringValue(right_operator);
}

void precedenceHandler::reorderExpression(std::shared_ptr < node > node_)
{
	if (node_->getNodeType() != nodeType::NODE_TYPE_EXPRESSION)
	{
		//might be number, identifier or string. We need an operator
		return;
	}

	if (node_->getLeftNode()->getNodeType() != nodeType::NODE_TYPE_EXPRESSION &&
		node_->getRightNode() && node_->getRightNode()->getNodeType() != nodeType::NODE_TYPE_EXPRESSION)
	{
		return;
	}

	if (node_->getLeftNode()->getNodeType() != nodeType::NODE_TYPE_EXPRESSION &&
		node_->getRightNode() && node_->getRightNode()->getNodeType() == nodeType::NODE_TYPE_EXPRESSION)
	{
		//operator on the right side
		if (precedence::leftOperatorHasPriority(node_->getStringValue(), node_->getRightNode()->getStringValue()))
		{
			shiftChildrenLeft(node_);
		}
	}
}
