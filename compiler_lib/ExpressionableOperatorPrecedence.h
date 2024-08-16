#ifndef _precedence_
#define _precedence_


#include <string>
#include <vector>
namespace precedence
{


enum precedenceAssocitivity
{
	ASSOSCIATIVITY_LEFT_TO_RIGHT,
	ASSOSCIATIVITY_RIGHT_TO_LEFT
};

struct precedenceGroup
{
	int precedence;
	precedenceAssocitivity associtivity;
	
};

extern std::vector<std::vector<std::string>> op_precedence = {
	{"++", "--", "()", "[]", "(", "[", ".", "->"}, // ASSOCIATIVITY_LEFT_TO_RIGHT
	{"*", "/", "%"}, // ASSOCIATIVITY_LEFT_TO_RIGHT
	{"+", "-"}, // ASSOCIATIVITY_LEFT_TO_RIGHT
	{"<<", ">>"}, // ASSOCIATIVITY_LEFT_TO_RIGHT		{"<", ">", "<=", ">="}, // ASSOCIATIVITY_LEFT_TO_RIGHT
	{"==", "!="}, // ASSOCIATIVITY_LEFT_TO_RIGHT
	{"&"}, // ASSOCIATIVITY_LEFT_TO_RIGHT
	{"^"}, // ASSOCIATIVITY_LEFT_TO_RIGHT
	{"|"}, // ASSOCIATIVITY_LEFT_TO_RIGHT
	{"&&"}, // ASSOCIATIVITY_LEFT_TO_RIGHT
	{"||"}, // ASSOCIATIVITY_LEFT_TO_RIGHT
	{"?", ":"}, // ASSOCIATIVITY_RIGHT_TO_LEFT
	{"=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|="}, // ASSOCIATIVITY_RIGHT_TO_LEFT
	{","} // ASSOCIATIVITY_LEFT_TO_RIGHT
};



precedenceGroup getPrecedence(const std::string& op)
{
	for (std::size_t i = 0; i < op_precedence.size(); ++i)
	{
		// Check if the operator is in the current row (vector of strings)
		if (std::find(op_precedence[i].begin(), op_precedence[i].end(), op) != op_precedence[i].end())
		{
			precedenceGroup group;
			group.precedence = static_cast<int>(i);
			if (i == 11 || i == 12)
				group.associtivity = ASSOSCIATIVITY_RIGHT_TO_LEFT;
			else
				group.associtivity = ASSOSCIATIVITY_LEFT_TO_RIGHT;
				
			return group; // Return the row index as an integer
		}
	}
	
}

bool leftOperatorHasPriority(std::string operator_left, std::string operator_right)
{
	if (operator_left == operator_right)
	{
		return false;
	}

	struct precedenceGroup group_left = getPrecedence(operator_left);
	struct precedenceGroup group_right = getPrecedence(operator_right);

	if (group_left.associtivity == ASSOSCIATIVITY_RIGHT_TO_LEFT)
	{
		false;
	}
	return group_left.precedence <= group_right.precedence;
}





}
#endif // !_precedence_