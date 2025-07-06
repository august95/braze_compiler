#include "pch.h"
#include "node.h"
#include "braze_compiler.h"



node::node()
	:m_body_size(0),
	m_node_type(nodeType::NODE_TYPE_BLANK),
	m_number_val(0),
	m_stack_offset(0)
{

}

node::node(filePosition file_position)
	:m_file_position(file_position),
	m_node_type(nodeType::NODE_TYPE_BLANK),
	m_body_size(0),
	m_number_val(0),
	m_stack_offset(0)
{
}

node::node(nodeType node_type, filePosition file_position)
	:m_node_type(node_type),
	m_file_position(file_position),
	m_body_size(0),
	m_number_val(0),
	m_stack_offset(0)
{

}


bool node::isValidExpressionType()
{
	return	m_node_type == NODE_TYPE_EXPRESSION ||
		m_node_type == NODE_TYPE_EXPRESSION_PARANTHESES ||
		m_node_type == NODE_TYPE_IDENTIFIER ||
		m_node_type == NODE_TYPE_UNARY ||
		m_node_type == NODE_TYPE_NUMBER ||
		m_node_type == NODE_TYPE_STRING;
}

int node::getDatatypeSize()
{
	//local variables in the scope increment the stack size of that scope,
	//function parameters is not yet parsed by the parser. The parameters
	//will be located above the previous base pointer on the stack. Will 
	//probably require an own stack size function
	if (m_node_type == NODE_TYPE_VARIABLE)
	{
		if (m_datatype)
		{
			return m_datatype->getDatatypeSize();
		}
	}
	cerror("retreived dataType size on non variable node");
	return 0;
}

void node::addStatement(std::shared_ptr<node> statement)
{
	//stack size calculation moved to node class, and is performed after all tokens has been parsed!
	m_statements.push_back(statement);
	//TODO: add code for padding and allignemt?
	//x86 acquire minimum 16 byte stack size
}



void node::calculateStackOffset(int& stack_offset)
{
	if (m_node_type == NODE_TYPE_VARIABLE)
	{
		stack_offset += m_datatype->getDatatypeSize();
		setStackOffset(stack_offset);
	}
	else if (m_node_type == NODE_TYPE_BODY)
	{
		//we might have nested scopes {{}}, stack_offset can be something else that 0 from the previous scope
		//store for nested body size calculation, 0 if first scope in function
		int stack_offset_copy = stack_offset; 
		setStackOffset(stack_offset);

		for (auto it = m_statements.begin(); it != m_statements.end(); ++it) 
		{
			std::shared_ptr<node> current_node = *it;
			current_node->calculateStackOffset(stack_offset);
		}
		m_body_size = stack_offset - stack_offset_copy;
	}
	else if (m_node_type == NODE_TYPE_FUNCTION)
	{
		//avoid adding the stack offset from the global variables
		int new_stack_offset = 0;
		//not forwared delcaration
		if (m_body_node)
		{
			m_body_node->calculateStackOffset(new_stack_offset);
			m_body_size = m_body_node->getBodySize();
		}
	}

}


void node::generateExpressionFlag()
{
  if (STRINGS_EQUAL(m_string_value.c_str(), "+"))
  {
    m_exp_type = EXPRESSION_IS_ADDITION;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "-"))
  {
    m_exp_type = EXPRESSION_IS_SUBTRACTION;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "*"))
  {
    m_exp_type = EXPRESSION_IS_MULTPILICATION;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "/"))
  {
    m_exp_type = EXPRESSION_IS_DIVISION;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "%"))
  {
    m_exp_type = EXPRESSION_IS_MODULUS;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), ">"))
  {
    m_exp_type = EXPRESSION_IS_ABOVE;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "<"))
  {
    m_exp_type = EXPRESSION_IS_BELOW;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), ">="))
  {
    m_exp_type = EXPRESSION_IS_ABOVE_OR_EQUAL;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "<="))
  {
    m_exp_type = EXPRESSION_IS_BELOW_OR_EQUAL;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "!="))
  {
    m_exp_type = EXPRESSION_IS_NOT_EQUAL;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "=="))
  {
    m_exp_type = EXPRESSION_IS_EQUAL;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "&&"))
  {
    m_exp_type = EXPRESSION_LOGICAL_AND;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "||"))
  {
    m_exp_type = EXPRESSION_LOGICAL_OR;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "<<"))
  {
    m_exp_type = EXPRESSION_BITSHIFT_LEFT;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), ">>"))
  {
    m_exp_type = EXPRESSION_BITSHIFT_RIGHT;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "&"))
  {
    m_exp_type = EXPRESSION_IS_BITWISE_AND;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "|"))
  {
    m_exp_type = EXPRESSION_IS_BITWISE_OR;
  }
  else if (STRINGS_EQUAL(m_string_value.c_str(), "^"))
  {
    m_exp_type = EXPRESSION_IS_BITWISE_XOR;
  }
}
