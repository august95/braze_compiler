#pragma once
#include "node.h"

class precedenceHandler
{
public:
	static void shiftChildrenLeft(std::shared_ptr < node > node_);
	static void reorderExpression(std::shared_ptr < node > node_);
};
