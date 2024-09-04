// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#define STRINGS_EQUAL(str, str2) \
    (str && str2 && (strcmp(str, str2) == 0))
#endif //PCH_H

/*
/*
void nodeExpresssion::shiftChildrenLeft(std::shared_ptr < nodeExpresssion > node)
{
	std::string right_operator = node->getRightNode()->getStringValue();
	std::shared_ptr < nodeExpresssion > new_left_exp_node = node->getLeftNode();
	std::shared_ptr < nodeExpresssion > new_right_exp_node = node->getLeftNode();

	std::shared_ptr < nodeExpresssion > new_left_operand =  std::make_shared< nodeExpresssion >(nodeType::NODE_TYPE_EXPRESSION, node->getFilePosition(), new_right_exp_node);
	std::shared_ptr < nodeExpresssion > new_right_operand = node->getRightNode()->getRightNode();
	node->setLeftNode(new_left_operand);
	node->setRightNode(new_right_operand);
	node->setStringValue(right_operator);

}


/*
TEST(precedence, precedence) {

	using namespace precedence;
	std::string op = "<<";
	std::string op2 = ",";
	EXPECT_FALSE(leftOperatorHasPriority("<<", "++"));
	EXPECT_TRUE(leftOperatorHasPriority("<<", "?"));
	precedenceGroup group = getPrecedence(op);
	EXPECT_EQ(group.precedence, 3);
	group = getPrecedence(op2);
	EXPECT_EQ(group.precedence, 12);

}

*/

/*
	ExpressionableOperatorPrecedence()
		: data{
			{{"++", "--", "()", "[]", "(", "[", ".", "->"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
			{{"*", "/", "%"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
			{{"+", "-"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
			{{"<<", ">>"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
			{{"<", ">", "<=", ">="}, ASSOCIATIVITY_LEFT_TO_RIGHT},
			{{"==", "!="}, ASSOCIATIVITY_LEFT_TO_RIGHT},
			{{"&"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
			{{"^"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
			{{"|"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
			{{"&&"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
			{{"||"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
			{{"?", ":"}, ASSOCIATIVITY_RIGHT_TO_LEFT},
			{{"=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|="}, ASSOCIATIVITY_RIGHT_TO_LEFT},
			{",", ASSOCIATIVITY_LEFT_TO_RIGHT}
		}
	{
*/
