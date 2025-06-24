#pragma onc

//receives a list of tokens, creates an abstract syntax tree with nodes

#include "token.h"
#include "node.h"
#include "datatype.h"
#include "scope.h"
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
	int startParser();
	std::list < std::shared_ptr < node > > getAbstractSyntaxTree() { return m_nodes; }
	std::shared_ptr < scope > lastScope() { return m_last_scope; }

private:
	//returns next tokens and pops the token of the token list
	std::shared_ptr < token > nextToken();
	//peeks at the next token
	std::shared_ptr < token > peekToken();
	std::shared_ptr < scope > newScope();
	void finishScope();
	void addNodeToCurrentScope(std::shared_ptr < node > node);
	void pushNode(std::shared_ptr < node > node);
	std::shared_ptr < node > peekLastNode() { if (!m_nodes.empty()) return m_nodes.back(); return std::shared_ptr < node >();	}
	std::shared_ptr < node > peekLastNodeExpect(nodeType node_type);
	std::shared_ptr < node > popLastNode() { auto node = m_nodes.back(); m_nodes.pop_back(); return node; }
	std::shared_ptr < node > makeExpressionNode(filePosition file_position, std::string operator_, 
													  	   std::shared_ptr < node > left_node, std::shared_ptr < node > right_node);

	template <class nodeType>
	std::shared_ptr <nodeType> cast_node(std::shared_ptr<node> node_);

	void parseTokens();
	void parseNextToken();
	

	void parseExpression();
	void parseExpressionOperatorOrOperand( bool& continue_to_parse_exp);
	void parseOperand();
	void parseOperator();
	void parseNormalExpression();
	void parseKeyword();
	void parseGlobalKeyword();
	void parseVariableOrFunction();
	void parseFunction();
	void parseBody();
	void parseStatement();
	void parseFunctionParameters();
	void parseSymbol();
	void parseUnary();

	std::shared_ptr < datatype > parseDatatype();


private:
	std::list < std::shared_ptr < node > > m_nodes;
	std::list < std::shared_ptr < token > > m_tokens;
	std::shared_ptr < scope > m_root_scope;
	std::shared_ptr < scope > m_last_scope;

	void _assert_(bool condition, std::string message);
	};


