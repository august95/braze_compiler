#pragma once
#include "node.h"

struct expresssionable_op_precedence_group {
	const char* operators[16]; // or whatever max size fits your needs
	int associtivity;
};

class precedenceHandler
{
public:
	static void shiftChildrenLeft(std::shared_ptr < node > node_);
	static void reorderExpression(std::shared_ptr < node > node_);
	static int getPrecendenceForOperator(const char* op, expresssionable_op_precedence_group** group_out);
	static bool leftOperatorHasPriority(const char* op_left, const char* op_right);
};
