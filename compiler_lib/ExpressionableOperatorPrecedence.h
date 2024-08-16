#include <string>
#include <vector>

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

std::vector<std::vector<std::string>> op_precedence = {
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
