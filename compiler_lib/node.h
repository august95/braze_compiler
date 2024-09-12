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


class node
{
public:
	node();
	node(filePosition file_position);
	node(nodeType node_type);
	node(nodeType node_type, filePosition file_position);
	node(nodeType node_type, filePosition file_position, std::string string_value);
	node(nodeType node_type, filePosition file_position, unsigned long number_value);
	node(nodeType node_type, filePosition file_position, std::shared_ptr < node > left_node, std::shared_ptr < node > right_node);

	void reorderExpression();
	bool isValidExpressionType();

	nodeType getNodeType() { return m_node_type;	}
	void setNodeType(nodeType node_type) { m_node_type = node_type; }
	filePosition getFilePosition() { return m_file_position; }

	std::shared_ptr < node > getLeftNode() { return m_left_node; }
	std::shared_ptr < node > getRightNode() { return m_right_node; }
	void setValueNode(std::shared_ptr < node > val_node) { m_value_node = val_node; }
	std::shared_ptr < node > getValueNode() { return m_value_node; }
	void setBodyNode(std::shared_ptr < node > body_node) { m_body_node = body_node; }
	std::shared_ptr < node > getBodyNode() { return m_body_node; }

	void setDatatype(std::shared_ptr < datatype > dtype) { m_datatype = dtype; }
	std::shared_ptr < datatype > getDatatype() {return m_datatype; }
	void setStatements(std::list < std::shared_ptr < node > > statements) { m_statements = statements; }
	std::list < std::shared_ptr < node > > getStatements() { return m_statements; }

	void setStringValue(std::string string_value) { m_string_value = string_value; }
	std::string getStringValue() { return m_string_value; }
	unsigned long getNumberValue() { return m_number_val; }

private:
	void setRightNode(std::shared_ptr < node > right_node) { m_right_node = right_node; }
	void setLeftNode(std::shared_ptr < node > left_node) { m_left_node = left_node; }
	void shiftChildrenLeft();

protected:
	//for expression nodes
	std::shared_ptr < node > m_left_node; 
	std::shared_ptr < node > m_right_node;

	//used by: variable nodes
	std::shared_ptr < node > m_value_node;
	std::shared_ptr < datatype > m_datatype;

	//used by: body nodes
	std::list < std::shared_ptr < node > > m_statements;

	//used by: function nodes
	std::shared_ptr < node > m_body_node;

	nodeType m_node_type;
	filePosition m_file_position;
	std::string m_string_value;
	unsigned long m_number_val;
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

