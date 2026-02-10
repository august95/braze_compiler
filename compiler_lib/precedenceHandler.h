#pragma once
#include "node/node.h"
#include "node/nodeExpression.h"
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
  static void reorderExpression(std::shared_ptr<nodeExpression> node_);
  static void shiftChildrenLeft(std::shared_ptr<nodeExpression> node_);
  static void moveRightLeftNodeToLeft(std::shared_ptr<nodeExpression> node_);
  static int getPrecendenceForOperator(const char *op, expresssionable_op_precedence_group **group_out);
  static bool leftOperatorHasPriority(const char *op_left, const char *op_right);
};
