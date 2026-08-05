#pragma once
#include "node_/node.h"
#include "node_/nodeExpression.h"
#include "braze_compiler.h"
#include <memory>

const static unsigned int TOTAL_OPERATOR_GROUPS = 14;
struct expresssionable_op_precedence_group
{
  const char *operators[TOTAL_OPERATOR_GROUPS];
  int associtivity;
};

class precedenceHandler
{
public:

  /*
changing order according to precedence:
    *
50     +
    30   20

needs to be
    +
  *    20
50  30

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

  template < class nodeClassExpression, nodeType NODE_TYPE_ENUM_EXPRESSION>
  static void reorderExpression(std::shared_ptr<nodeClassExpression> node_)
  {
    if (node_->getNodeType() != NODE_TYPE_ENUM_EXPRESSION)
    {
      // might be number, identifier or string. We need an operator
      return;
    }

    if (node_->getLeftNode()->getNodeType() != NODE_TYPE_ENUM_EXPRESSION &&
      node_->getRightNode() && node_->getRightNode()->getNodeType() != NODE_TYPE_ENUM_EXPRESSION)
    {
      return;
    }

    if (node_->getLeftNode()->getNodeType() != NODE_TYPE_ENUM_EXPRESSION &&
      node_->getRightNode() && node_->getRightNode()->getNodeType() == NODE_TYPE_ENUM_EXPRESSION)
    {
      // operator on the right side
      if (precedenceHandler::leftOperatorHasPriority(node_->getStringValue().c_str(), node_->getRightNode()->getStringValue().c_str()))
      {
        shiftChildrenLeft< nodeClassExpression, NODE_TYPE_ENUM_EXPRESSION >(node_);
      }
    }
    //avoid bloating nodePreProcessor with isArray() and isAssignmentNode() to make compiler happy
    //moveRightLeftNodeToLeft are only needed for nodeExpression
    if (std::shared_ptr<nodeExpression> node_expression = try_cast_node< nodeExpression>(node_))
    {
      if ((node_expression->getLeftNode()->isArray() || node_expression->getRightNode()->isAssignmentNode()) ||
        ((node_->getLeftNode()->getNodeType() == NODE_TYPE_ENUM_EXPRESSION && STRINGS_EQUAL(node_->getLeftNode()->getStringValue().c_str(), "()")) &&
          (node_->getLeftNode()->getNodeType() == NODE_TYPE_ENUM_EXPRESSION && STRINGS_EQUAL(node_->getLeftNode()->getStringValue().c_str(), ","))))
      {
        moveRightLeftNodeToLeft < nodeClassExpression, NODE_TYPE_ENUM_EXPRESSION > (node_);
      }
    }
  }

  template < class nodeClassExpression, nodeType NODE_TYPE_ENUM_EXPRESSION>
  static void shiftChildrenLeft(std::shared_ptr<nodeClassExpression> node_)
  {
    std::string right_operator = node_->getRightNode()->getStringValue();
    std::shared_ptr<nodeClassExpression> new_left_exp_node = node_->getLeftNode();
    std::shared_ptr<nodeClassExpression> new_right_exp_node = node_->getRightNode()->getLeftNode();

    std::shared_ptr<nodeClassExpression> new_left_operand = std::make_shared<nodeClassExpression>(NODE_TYPE_ENUM_EXPRESSION, node_->getFilePosition());
    new_left_operand->setLeftNode(new_left_exp_node);
    new_left_operand->setRightNode(new_right_exp_node);
    new_left_operand->setStringValue(node_->getStringValue());
    std::shared_ptr<nodeClassExpression> new_right_operand = node_->getRightNode()->getRightNode();
    node_->setLeftNode(new_left_operand);
    node_->setRightNode(new_right_operand);
    node_->setStringValue(right_operator);
  }

  template < class nodeClassExpression, nodeType NODE_TYPE_ENUM_EXPRESSION>
  static void moveRightLeftNodeToLeft(std::shared_ptr<nodeClassExpression> node_)
  {
    std::shared_ptr<nodeClassExpression> new_left_node = std::make_shared<nodeClassExpression>(NODE_TYPE_ENUM_EXPRESSION, node_->getFilePosition());
    new_left_node->setLeftNode(node_->getLeftNode());
    new_left_node->setRightNode(node_->getRightNode()->getLeftNode());
    new_left_node->setStringValue(node_->getStringValue());

    std::string new_op = node_->getRightNode()->getStringValue();
    node_->setLeftNode(new_left_node);
    node_->setRightNode(node_->getRightNode()->getRightNode());
    node_->setStringValue(new_op);
  }

  static int getPrecendenceForOperator(const char *op, expresssionable_op_precedence_group **group_out);
  static bool leftOperatorHasPriority(const char *op_left, const char *op_right);
};
