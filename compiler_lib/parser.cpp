#include "pch.h"
#include "parser.h"
#include "braze_compiler.h"
#include <memory>



parser::parser()
{

}

void parser::startParser()
{
	parseTokens();
}

std::shared_ptr < token > parser::nextToken()
{
	std::shared_ptr < token > token = m_tokens.front();
	m_tokens.pop_front();
	return token;
}

std::shared_ptr < token > parser::peekToken()
{
	if (m_tokens.empty())
		return std::make_shared < token >();
	return m_tokens.front();
}

void parser::pushNode(std::shared_ptr < node > node)
{
	m_nodes.push_back(node);
}

std::shared_ptr < node > parser::peekLastNodeExpect(nodeType node_type)
{
	std::shared_ptr < node > node = peekLastNode();
	_assert_(node->getNodeType() & node_type, "expected other node type");
	return node;
}

std::shared_ptr < node > parser::makeExpressionNode(filePosition file_position, std::string operator_, std::shared_ptr < node > left_node, std::shared_ptr < node > right_node)
{

	std::shared_ptr < node > expression_node = std::make_shared<node>(NODE_TYPE_EXPRESSION, file_position, left_node, right_node);
	expression_node->setStringValue(operator_);
	return expression_node;
}

template <class nodeType>
std::shared_ptr<nodeType> parser::cast_node(std::shared_ptr<node> node_)
{
	// Use std::static_pointer_cast<nodeType> to cast from base class to derived class
	if (!node_)
		return std::shared_ptr<nodeType>();
	std::shared_ptr<nodeType> cast_node_ = std::static_pointer_cast<nodeType>(node_);
	return cast_node_;
}

void parser::parseTokens()
{
	while (!m_tokens.empty())
	{
		parseNextToken();
	}
}

void parser::parseNextToken()
{
	std::shared_ptr < token > token = peekToken();
	if (token->isTokenTypeNumber() || token->isTokenTypeIdentifier() ||	token->isTokenTypeString())
	{
		parseExpression();
	}
	if (token->isTokenTypeKeyword())
	{
		parseGlobalKeyword();
	}
}

void parser::parseExpression()
{
	//parse all the tokens in the expression until the ';'. Example :a = b + 5;
 	while (parseExpressionOperatorOrOperand() == 0)
	{

	}
}

int parser::parseExpressionOperatorOrOperand()
{
	std::shared_ptr < token > token = peekToken();
	if (!token)
	{
		return -1;
	}
	if (token->isTokenTypeNumber() || token->isTokenTypeIdentifier())
	{
		parseSingleTokenToExpresssionNode();
		return 0;
	}
	else if (token->isTokenTypeOperator())
	{
		parseOperand();
		return 0;
	}
	//TODO: add keyword in case of cast:  int a = (int)b;

	return 1;
}

void parser::parseSingleTokenToExpresssionNode()
{
	std::shared_ptr < token > t = nextToken();
	std::shared_ptr < node > n;

	if (t->isTokenTypeNumber())
	{
		n = std::make_shared<node>(nodeType::NODE_TYPE_NUMBER, t->getFilePosition(), t->getNumberValue());
	}
	else if (t->isTokenTypeIdentifier())
	{
		n = std::make_shared<node>(nodeType::NODE_TYPE_IDENTIFIER, t->getFilePosition(), t->getStringValue());
	}
	else if (t->isTokenTypeString())
	{
		n = std::make_shared<node>(nodeType::NODE_TYPE_STRING, t->getFilePosition(), t->getStringValue());
	}	
	pushNode(n);	
}

void parser::parseOperand()
{
	parseNormalExpression();
}

void parser::parseNormalExpression()
{
	// 50 * 30 + 20 
	std::shared_ptr < node > left_node = peekLastNode(); //50  
	std::shared_ptr <token> operatort_token = peekToken(); // *
	//std::string operator_ = operatort_token->getStringValue();

	if (!left_node || !left_node->isValidExpressionType())
	{
		//TODO: might be unary: *a, &a or !a
		// 
		//FIXME: create debug messages that separates LEXING from PARSING and prints error location in file
		//cwarning("left node in expression %i is not allowed in expression", left_node->getNodeType());
		return;
	}
	nextToken(); // operator token popped '*'
	popLastNode();  // 50
	parseExpression(); // parse 30 + 20
	std::shared_ptr < node > right_node = popLastNode(); // + L(30) R(20)
	std::shared_ptr < node > expression_node = makeExpressionNode(operatort_token->getFilePosition(), operatort_token->getStringValue(), left_node, right_node);
	
	expression_node->reorderExpression();

	pushNode(expression_node);
}

void parser::parseKeyword()
{
	std::shared_ptr<token> token = peekToken();

	if (datatype::isKeywordVariableModifier(token->getStringValue()) || datatype::IsKeywordDatatype(token->getStringValue()))
	{
		//static const int func_or_variable ...
		parseVariableOrFunction();

	}
}

void parser::parseGlobalKeyword()
{
	//FIXME: add symbol resolver

	parseKeyword();
	nextToken();
}

void parser::parseVariableOrFunction()
{
	//to parse: static const long long*** var_name or  int function_name()

	std::shared_ptr<datatype> datatype = parseDatatype();

	if (datatype->isStruct() || datatype->isUnion())
	{
		//#warning "struct not implemented"
		//If global scope, parse struct declaration or function with struct return type
		//If inside statement, parse struct variable'
		//
	}


	std::shared_ptr<token> token = nextToken();
	assert(token->isTokenTypeIdentifier() && "expected variable name or function name");
	std::shared_ptr < node > _node = std::make_shared < node >(token->getFilePosition());
	std::string variable_or_function_name = token->getStringValue();
	_node->setStringValue(variable_or_function_name);

	token = nextToken();
	if (token->isTokenTypeSymbol() && (token->getCharValue() == ';')) 
	{
		// unassigned variable int a;
		_node->setNodeType(nodeType::NODE_TYPE_VARIABLE);
		_node->setDatatype(datatype);
	}
	else if (token->isTokenTypeOperator() && (token->getStringValue() == "="))
	{
		//declared variable int a = 50;
		_node->setNodeType(nodeType::NODE_TYPE_VARIABLE);
		_node->setDatatype(datatype);
		parseExpression();
		_node->setValueNode(popLastNode());
	}
	else if (token->isTokenTypeOperator() && (token->getStringValue() == "("))
	{
		//parsing function int a(){}                
		_node->setNodeType(nodeType::NODE_TYPE_FUNCTION);
		parseBody();
	}
	else
	{
		cerror("expected function or variable delcaration");
		assert(false);
	}
	pushNode(_node);

}

void parser::parseBody()
{
	//create new scope

	//pop '{'
	
	//parse all statements

	//pop '}'

	//end scopes
}

void parser::parseStatement()
{
	
	//pop token

		//parse keyword
		//parse expression
		//parse symbol ('{' new scope or ':' label)
		//no other token types is expected as the first token in the statement

	// case token type number, token type identifier, if, else, switch, return, goto, for, while....
	
	//pop ';'
}

void parser::parseSymbol()
{
	//parse '{' new scope

	//parse ':' label
}

std::shared_ptr<datatype> parser::parseDatatype()
{
	//to parse: static const long long*** name....
	std::shared_ptr<token> token = peekToken();
	std::shared_ptr<datatype> dtype = std::make_shared < datatype > (token->getFilePosition());

	//static const
	while (datatype::isKeywordVariableModifier(token->getStringValue()))
	{
		token = nextToken();
		dtype->setKeyVariableModifier(token->getStringValue());
		token = peekToken();
	}

	//long long
	while (datatype::IsKeywordDatatype(token->getStringValue()))
	{
		token = nextToken();
		dtype->setDataType(token->getStringValue());
		token = peekToken();
	}
	//***
	while (token->isTokenTypeOperator() && STRINGS_EQUAL(token->getStringValue().c_str(), "*"))
	{
		token = nextToken();
		dtype->incrementPointerDepth();
		token = peekToken();
	}

	return dtype;
}


void parser::_assert_(bool condition, std::string message)
{
	if (!condition)
	{
		if (!message.empty())
		{
			cerror(message.c_str());
		}
	}
	assert(condition);
};
