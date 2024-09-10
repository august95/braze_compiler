#pragma onc

//receives a list of tokens, creates an abstract syntax tree with nodes

#include "token.h"
#include "node.h"
#include "datatype.h"
#include <list>
#include <iostream>

/*
* FIXME: 
* Add parsing of unary operator in parseNormalExpression.
* Add rules for parsing expressions with [, (, ?, ','. And reorder expression if needed.* 
*/

class parser
{
public:
	parser();

	void setTokenList(std::list < std::shared_ptr < token > > tokens) { m_tokens = tokens; };
	void startParser();
	std::list < std::shared_ptr < node > > getAbstractSyntaxTree() { return m_nodes; }

private:
	std::shared_ptr < token > nextToken();
	std::shared_ptr < token > peekToken();
	void pushNode(std::shared_ptr < node > node);
	std::shared_ptr < node > peekLastNode() { return m_nodes.back(); }
	std::shared_ptr < node > peekLastNodeExpect(nodeType node_type);
	std::shared_ptr < node > popLastNode() { auto node = m_nodes.back(); m_nodes.pop_back(); return node; }
	std::shared_ptr < node > makeExpressionNode(filePosition file_position, std::string operator_, 
													  	   std::shared_ptr < node > left_node, std::shared_ptr < node > right_node);

	template <class nodeType>
	std::shared_ptr <nodeType> cast_node(std::shared_ptr<node> node_);

	void parseTokens();
	void parseNextToken();
	

	void parseExpression();
	int parseExpressionOperatorOrOperand();
	void parseSingleTokenToExpresssionNode();
	void parseOperand();
	void parseNormalExpression();
	void parseKeyword();
	void parseGlobalKeyword();
	void parseVariableOrFunction();
	void parseBody();
	void parseStatement();
	void parseSymbol();

	std::shared_ptr < datatype > parseDatatype();


private:
	std::list < std::shared_ptr < node > > m_nodes;
	std::list < std::shared_ptr < token > > m_tokens;

	void _assert_(bool condition, std::string message);


};


