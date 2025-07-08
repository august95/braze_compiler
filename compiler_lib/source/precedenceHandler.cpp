#include "pch.h"
#include "../precedenceHandler.h"
#include <cstddef>


#define ASSOSCIATIVITY_LEFT_TO_RIGHT 0
#define ASSOSCIATIVITY_RIGHT_TO_LEFT 1

expresssionable_op_precedence_group op_precedence[TOTAL_OPERATOR_GROUPS] = {
	{{"++", "--", "()", "[]", "(", "[", ".", "->", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
	{{"*", "/", "%", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
	{{"+", "-", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
	{{"<<", ">>", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
	{{"<", ">", "<=", ">=", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
	{{"==", "!=", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
	{{"&", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
	{{"^", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
	{{"|", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
	{{"&&", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
	{{"||", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
	{{"?", ":", NULL}, ASSOSCIATIVITY_RIGHT_TO_LEFT},
	{{"=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|=", NULL}, ASSOSCIATIVITY_RIGHT_TO_LEFT},
	{{",", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT}
};

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
		if (precedenceHandler::leftOperatorHasPriority(node_->getStringValue().c_str(), node_->getRightNode()->getStringValue().c_str()))
		{
			shiftChildrenLeft(node_);
		}
	}
	if ((node_->getLeftNode()->isArray() || node_->getRightNode()->isAssignmentNode()) ||
			((node_->getLeftNode()->getNodeType() == NODE_TYPE_EXPRESSION && STRINGS_EQUAL(node_->getLeftNode()->getStringValue().c_str(), "()")) &&
				(node_->getLeftNode()->getNodeType() == NODE_TYPE_EXPRESSION && STRINGS_EQUAL(node_->getLeftNode()->getStringValue().c_str(), ","))))
	{
    moveRightLeftNodeToLeft(node_);
	}
}



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

void precedenceHandler::moveRightLeftNodeToLeft(std::shared_ptr<node> node_)
{
  std::shared_ptr < node > new_left_node = std::make_shared< node >(nodeType::NODE_TYPE_EXPRESSION, node_->getFilePosition());
  new_left_node->setLeftNode(node_->getLeftNode());
  new_left_node->setRightNode(node_->getRightNode()->getLeftNode());
  new_left_node->setStringValue(node_->getStringValue());

  std::string new_op = node_->getRightNode()->getStringValue();
  node_->setLeftNode(new_left_node);
  node_->setRightNode(node_->getRightNode()->getRightNode());
  node_->setStringValue(new_op);
}


int precedenceHandler::getPrecendenceForOperator(const char* op, expresssionable_op_precedence_group** group_out)
{
	*group_out = nullptr;
	for (int i = 0; i < TOTAL_OPERATOR_GROUPS; i++)
	{
		for (int b = 0; op_precedence[i].operators[b]; b++)
		{
			const char* _op = op_precedence[i].operators[b];
			if (STRINGS_EQUAL(op, _op))
			{
				*group_out = &op_precedence[i];
				return i;
			}
		}
	}
	return -1;
}

bool precedenceHandler::leftOperatorHasPriority(const char* op_left, const char* op_right)
{
	expresssionable_op_precedence_group* group_left = nullptr;
	expresssionable_op_precedence_group* group_right = nullptr;

	if (STRINGS_EQUAL(op_left, op_right))
	{
		return false;
	}

	int precedence_left = getPrecendenceForOperator(op_left, &group_left);
	int precedence_right = getPrecendenceForOperator(op_right, &group_right);

	if (precedence_left == -1 || precedence_right == -1)
	{
		return false; 
	}

	if (group_left->associtivity == ASSOSCIATIVITY_RIGHT_TO_LEFT)
	{
		return precedence_left < precedence_right;
	}

	return precedence_left <= precedence_right;
}
