#pragma once
//node created by the parser,  is leafs in the abstract syntax tree

#include <string>
#include "filePosition.h"
#include "datatype.h"
#include <iostream>
#include <list>

enum nodeType
{
	NODE_TYPE_UNDEFINED,
	NODE_TYPE_EXPRESSION,
	NODE_TYPE_EXPRESSION_PARANTHESES,
	NODE_TYPE_NUMBER,
	NODE_TYPE_IDENTIFIER,
	NODE_TYPE_STRING,
	NODE_TYPE_VARIABLE,
	NODE_TYPE_VARIABLE_LIST,
	NODE_TYPE_LIST,
	NODE_TYPE_FUNCTION,
	NODE_TYPE_BODY,
	NODE_TYPE_STATEMENT_RETURN,
	NODE_TYPE_STATEMENT_IF,
	NODE_TYPE_STATEMENT_ELSE,
	NODE_TYPE_STATEMENT_WHILE,
	NODE_TYPE_STATEMENT_DO_WHILE,
	NODE_TYPE_STATEMENT_FOR,
	NODE_TYPE_STATEMENT_BREAK,
	NODE_TYPE_STATEMENT_CONTINUE,
	NODE_TYPE_STATEMENT_SWITCH,
	NODE_TYPE_STATEMENT_CASE,
	NODE_TYPE_STATEMENT_DEFAULT,
	NODE_TYPE_STATEMENT_GOTO,
	NODE_TYPE_UNARY,
	NODE_TYPE_TENARY,
	NODE_TYPE_LABEL,
	NODE_TYPE_STRUCT,
	NODE_TYPE_UNION,
	NODE_TYPE_BRACKET,
	NODE_TYPE_CAST,
	NODE_TYPE_BLANK
};


enum ExpressionType
{
	EXPRESSION_FLAG_RIGH_NODE = 0b00000000000000000000000000000001,
	EXPRESSION_IN_FUNCTION_CALL_ARGUMENTS = 0b00000000000000000000000000000010,
	EXPRESSION_IN_FUNCTION_CALL_LEFT_OPERAND = 0b00000000000000000000000000000100,
	EXPRESSION_IS_ADDITION = 0b00000000000000000000000000001000,
	EXPRESSION_IS_SUBTRACTION = 0b00000000000000000000000000010000,
	EXPRESSION_IS_MULTPILICATION = 0b00000000000000000000000000100000,
	EXPRESSION_IS_DIVISION = 0b00000000000000000000000001000000,
	EXPRESSION_IS_FUNCTION_CALL = 0b00000000000000000000000010000000,
	EXPRESSION_INDIRECTION = 0b00000000000000000000000100000000,
	EXPRESSION_GET_ADDRESS = 0b00000000000000000000001000000000,
	EXPRESSION_IS_ABOVE = 0b00000000000000000000010000000000,
	EXPRESSION_IS_ABOVE_OR_EQUAL = 0b00000000000000000000100000000000,
	EXPRESSION_IS_BELOW = 0b00000000000000000001000000000000,
	EXPRESSION_IS_BELOW_OR_EQUAL = 0b00000000000000000010000000000000,
	EXPRESSION_IS_EQUAL = 0b00000000000000000100000000000000,
	EXPRESSION_IS_NOT_EQUAL = 0b00000000000000001000000000000000,
	EXPRESSION_LOGICAL_AND = 0b00000000000000010000000000000000,
	EXPRESSION_LOGICAL_OR = 0b00000000000000100000000000000000,
	EXPRESSION_IN_LOGICAL_EXPRESSION = 0b00000000000001000000000000000000,
	EXPRESSION_BITSHIFT_LEFT = 0b00000000000010000000000000000000,
	EXPRESSION_BITSHIFT_RIGHT = 0b00000000000100000000000000000000,
	EXPRESSION_IS_BITWISE_OR = 0b00000000001000000000000000000000,
	EXPRESSION_IS_BITWISE_AND = 0b00000000010000000000000000000000,
	EXPRESSION_IS_BITWISE_XOR = 0b00000000100000000000000000000000,
	EXPRESSION_IS_NOT_ROOT_NODE = 0b00000001000000000000000000000000,
	EXPRESSION_IS_ASSIGNMENT = 0b00000010000000000000000000000000,
	IS_ALONE_STATEMENT = 0b00000100000000000000000000000000,
	EXPRESSION_IS_UNARY = 0b00001000000000000000000000000000,
	IS_STATEMENT_RETURN = 0b00010000000000000000000000000000,
	IS_RIGHT_OPERAND_OF_ASSIGNMENT = 0b00100000000000000000000000000000,
	IS_LEFT_OPERAND_OF_ASSIGNMENT = 0b01000000000000000000000000000000,
	EXPRESSION_IS_MODULUS = 0b10000000000000000000000000000000,
};

#define EXPRESSION_GEN_MATHABLE (       \
    EXPRESSION_IS_ADDITION |           \
    EXPRESSION_IS_SUBTRACTION |        \
    EXPRESSION_IS_MULTPILICATION |     \
    EXPRESSION_IS_DIVISION |           \
    EXPRESSION_IS_MODULUS |            \
    EXPRESSION_IS_FUNCTION_CALL |      \
    EXPRESSION_INDIRECTION |           \
    EXPRESSION_GET_ADDRESS |           \
    EXPRESSION_IS_ABOVE |              \
    EXPRESSION_IS_ABOVE_OR_EQUAL |     \
    EXPRESSION_IS_BELOW |              \
    EXPRESSION_IS_BELOW_OR_EQUAL |     \
    EXPRESSION_IS_EQUAL |              \
    EXPRESSION_IS_NOT_EQUAL |          \
    EXPRESSION_LOGICAL_AND |           \
    EXPRESSION_LOGICAL_OR |            \
    EXPRESSION_IN_LOGICAL_EXPRESSION | \
    EXPRESSION_BITSHIFT_LEFT |      \
    EXPRESSION_BITSHIFT_RIGHT |     \
    EXPRESSION_IS_BITWISE_OR |         \
    EXPRESSION_IS_BITWISE_AND |        \
    EXPRESSION_IS_BITWISE_XOR)

#define EXPRESSION_LOGICAL_OPERATOR (       \
    EXPRESSION_LOGICAL_AND |           \
    EXPRESSION_LOGICAL_OR)




class node
{
public:
	node();
	node(filePosition file_position);
	node(nodeType node_type, filePosition file_position);

	bool isValidExpressionType();
	int getDatatypeSize();

	nodeType getNodeType() { return m_node_type;	}
	void setNodeType(nodeType node_type) { m_node_type = node_type; }
	filePosition getFilePosition() { return m_file_position; }

	std::shared_ptr < node > getLeftNode() { return m_left_node; }
	void setLeftNode(std::shared_ptr < node > left_node) { m_left_node = left_node; }
	std::shared_ptr < node > getRightNode() { return m_right_node; }
	void setRightNode(std::shared_ptr < node > right_node) { m_right_node = right_node; }
	void setValueNode(std::shared_ptr < node > val_node) { m_value_node = val_node; }
	std::shared_ptr < node > getValueNode() { return m_value_node; }
	void setBodyNode(std::shared_ptr < node > body_node) { m_body_node = body_node; }
	std::shared_ptr < node > getBodyNode() { return m_body_node; }
	int getBodySize() { return m_body_size; }
	void setDeclarationNode(std::shared_ptr < node > declaration_node) { m_declaration_node = declaration_node; }
	std::shared_ptr < node > getDeclarationNode() { return m_declaration_node; }

	void setDatatype(std::shared_ptr < datatype > dtype) { m_datatype = dtype; }
	std::shared_ptr < datatype > getDatatype();
	void setReturnDatatype(std::shared_ptr < datatype > dtype) { m_return_datatype = dtype; }
	std::shared_ptr < datatype > getReturnDatatype() {return m_return_datatype; }
	void addStatement(std::shared_ptr<node> statement);
//	void setStatements(std::list < std::shared_ptr < node > > statements);
	std::list < std::shared_ptr < node > > getStatements() { return m_statements; }

	void setStringValue(std::string string_value) { m_string_value = string_value; }
	std::string getStringValue() { return m_string_value; }
	void setNumberValue(unsigned long number_val) { m_number_val = number_val; }
	unsigned long getNumberValue() { return m_number_val; }
	void setStackOffset(int stack_offset) { m_stack_offset = stack_offset; }
	int getStackOffset() { return m_stack_offset; }
	void generateExpressionFlag();
	ExpressionType getExpressionType() { return m_exp_type; }

	void calculateStackOffset(int& stack_offset);


    bool isArray() { return m_node_type == nodeType::NODE_TYPE_EXPRESSION && m_string_value == "[]"; }
    bool isAssignmentNode() { return m_node_type == nodeType::NODE_TYPE_EXPRESSION && (m_string_value == "=" || m_string_value == "+=" || m_string_value == "-=" || m_string_value == "/=" || m_string_value == "*="); }

	void setIsGlobal(bool is_global) { m_is_global = is_global; }	
	bool getIsGlobal() { return m_is_global; }
protected:
	//for expression nodes
	std::shared_ptr < node > m_left_node; 
	std::shared_ptr < node > m_right_node;

	//used by: variable nodes
	std::shared_ptr < node > m_value_node;
	std::shared_ptr < datatype > m_datatype;

	//used by: body nodes
	std::list < std::shared_ptr < node > > m_statements;
	int m_body_size;

	//used by: function nodes
	std::shared_ptr < node > m_body_node;
	std::shared_ptr < datatype > m_return_datatype;
	bool m_is_global;

	//used by identifiers
	std::shared_ptr < node > m_declaration_node;

	nodeType m_node_type;
	filePosition m_file_position;
	std::string m_string_value;
	unsigned long m_number_val;
	int m_stack_offset;
	ExpressionType m_exp_type;
};


/*
 for future use


enum nodeType
{
	NODE_TYPE_UNDEFINED					= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_EXPRESSION				= 0b0000000000000000000000000000000000000001,,
	NODE_TYPE_EXPRESSION_PARANTHESES	= 0b0000000000000000000000000000000000000101,
	NODE_TYPE_NUMBER					= 0b0000000000000000000000000000000000001001,
	NODE_TYPE_IDENTIFIER				= 0b0000000000000000000000000000000000010001,
	NODE_TYPE_STRING					= 0b0000000000000000000000000000000000100001,
	NODE_TYPE_VARIABLE					= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_VARIABLE_LIST				= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_LIST						= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_FUNCTION					= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_BODY						= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_STATEMENT_RETURN			= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_STATEMENT_IF				= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_STATEMENT_ELSE			= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_STATEMENT_WHILE			= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_STATEMENT_DO_WHILE		= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_STATEMENT_FOR				= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_STATEMENT_BREAK			= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_STATEMENT_CONTINUE		= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_STATEMENT_SWITCH			= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_STATEMENT_CASE			= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_STATEMENT_DEFAULT			= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_STATEMENT_GOTO			= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_UNARY						= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_TENARY					= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_LABEL						= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_STRUCT					= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_UNION						= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_BRACKET					= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_CAST						= 0b0000000000000000000000000000000000000000,
	NODE_TYPE_BLANK						= 0b0000000000000000000000000000000000000000,
};
*/

