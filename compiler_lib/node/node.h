#pragma once
// node created by the parser,  is leafs in the abstract syntax tree

#include <string>
#include "../filePosition.h"
#include "../datatype.h"
#include <iostream>
#include <list>
#include <memory>






enum nodeType : std::uint64_t
{
  NODE_TYPE_UNDEFINED               = 0x0000000000,
  
  NODE_TYPE_EXPRESSION              = 0x0000000080,
  NODE_TYPE_EXPRESSION_PARANTHESES  = 0x0000000001 | NODE_TYPE_EXPRESSION,
  NODE_TYPE_NUMBER                  = 0x0000000002 | NODE_TYPE_EXPRESSION,
  NODE_TYPE_IDENTIFIER              = 0x0000000004 | NODE_TYPE_EXPRESSION,
  NODE_TYPE_STRING                  = 0x0000000008 | NODE_TYPE_EXPRESSION,
  NODE_TYPE_UNARY                   = 0x0000000010 | NODE_TYPE_EXPRESSION,
  NODE_TYPE_TENARY                  = 0x0000000020 | NODE_TYPE_EXPRESSION,
  
  NODE_TYPE_STATEMENT               = 0x0000800000,
  NODE_TYPE_STATEMENT_GOTO          = 0x0000000100 | NODE_TYPE_STATEMENT,
  NODE_TYPE_STATEMENT_RETURN        = 0x0000000200 | NODE_TYPE_STATEMENT,
  NODE_TYPE_STATEMENT_IF            = 0x0000000400 | NODE_TYPE_STATEMENT,
  NODE_TYPE_STATEMENT_ELSE          = 0x0000000800 | NODE_TYPE_STATEMENT,
  NODE_TYPE_STATEMENT_WHILE         = 0x0000001000 | NODE_TYPE_STATEMENT,
  NODE_TYPE_STATEMENT_DO_WHILE      = 0x0000002000 | NODE_TYPE_STATEMENT,
  NODE_TYPE_STATEMENT_FOR           = 0x0000004000 | NODE_TYPE_STATEMENT,
  NODE_TYPE_STATEMENT_BREAK         = 0x0000008000 | NODE_TYPE_STATEMENT,
  NODE_TYPE_STATEMENT_CONTINUE      = 0x0000010000 | NODE_TYPE_STATEMENT,
  NODE_TYPE_STATEMENT_SWITCH        = 0x0000020000 | NODE_TYPE_STATEMENT,
  NODE_TYPE_STATEMENT_CASE          = 0x0000040000 | NODE_TYPE_STATEMENT,
  NODE_TYPE_STATEMENT_DEFAULT       = 0x0000080000 | NODE_TYPE_STATEMENT,
  
  //ad decl at                        0x0080000000
  NODE_TYPE_VARIABLE                = 0x0001000000,
  NODE_TYPE_VARIABLE_LIST           = 0x0002000000,
  NODE_TYPE_LIST                    = 0x0004000000,
  NODE_TYPE_BODY                    = 0x0100000000,
  NODE_TYPE_FUNCTION                = 0x0200000000,
  
  NODE_TYPE_LABEL                   = 0x4000000000,
  NODE_TYPE_STRUCT                  = 0x8000000000,
  NODE_TYPE_UNION                   = 0x1000000000,
  NODE_TYPE_BRACKET                 = 0x2000000000,
  NODE_TYPE_CAST                    = 0x4000000000,
  NODE_TYPE_BLANK                   = 0x8000000000

};


class node
{
public:
  node();
  node(filePosition file_position);
  node(nodeType node_type, filePosition file_position);
   

  int getDatatypeSize();
  filePosition getFilePosition() { return m_file_position; }

  nodeType getNodeType() { return m_node_type; }
  void setNodeType(nodeType node_type) { m_node_type = node_type; }

  void setValueNode(std::shared_ptr<node> val_node) { m_value_node = val_node; }
  std::shared_ptr<node> getValueNode() { return m_value_node; }
  virtual void setBodyNode(std::shared_ptr<node> body_node) { m_body_node = body_node; }
  std::shared_ptr<node> getBodyNode() { return m_body_node; }
  int getBodySize() { return m_body_size; }
  void setDeclarationNode(std::shared_ptr<node> declaration_node) { m_declaration_node = declaration_node; }
  std::shared_ptr<node> getDeclarationNode() { return m_declaration_node; }


  virtual void setDatatype(std::shared_ptr<datatype> dtype) { m_datatype = dtype; }
  std::shared_ptr<datatype> getDatatype();
  void setReturnDatatype(std::shared_ptr<datatype> dtype) { m_return_datatype = dtype; }
  std::shared_ptr<datatype> getReturnDatatype() { return m_return_datatype; }
  void addStatement(std::shared_ptr<node> statement);
  //  void setStatements(std::list < std::shared_ptr < node > > statements);
  std::list<std::shared_ptr<node>> getStatements() { return m_statements; }


  void setStackOffset(int stack_offset) { m_stack_offset = stack_offset; }
  int getStackOffset() { return m_stack_offset; }
  void generateExpressionFlag();

  void addFunctionArgumentNode(std::shared_ptr<node> node) { m_function_arguemnt.push_back(node); }
  std::list<std::shared_ptr<node>> getFunctionArguments() { return m_function_arguemnt; }
  void setIsFunctionArgument(bool is_function_argument) { m_is_function_argument = is_function_argument; }
  bool getIsFunctionArgument() { return m_is_function_argument; }


  virtual void calculateStackOffset(int &stack_offset);

  void setIsGlobal(bool is_global) { m_is_global = is_global; }
  bool getIsGlobal() { return m_is_global; }
  bool isFunctionPrototype() { return m_function_prototype; }
  void setFunctionPrototype(bool function_prototype) { m_function_prototype = function_prototype; }
  void setUnaryIndirectionDepth(int unary_indirection_depth) { m_unary_indirection_depth = unary_indirection_depth; }
  int getUnaryIndirectionDepth() { return m_unary_indirection_depth;  }
  virtual void setStringValue(std::string string_value) { m_string_value = string_value; } //FIXME resolve dtype owership with other datatype owners
  std::string getStringValue() { return m_string_value; }

protected:

  // used by: variable nodes
  std::shared_ptr<node> m_value_node;
  std::shared_ptr<datatype> m_datatype;
  bool m_is_global;
  bool m_is_function_argument;
  int m_padding;

  // used by: variable nodes and body nodes
  int m_stack_offset;

  // used by: body nodes
  std::list<std::shared_ptr<node>> m_statements;
  int m_body_size;

  // used by: function nodes
  std::shared_ptr<node> m_body_node;
  std::shared_ptr<datatype> m_return_datatype;
  std::list<std::shared_ptr<node>> m_function_arguemnt;
  int m_stack_size;
  bool m_function_prototype;
  // size of ebp and esp, migth be bigger when returning structs
  int m_stack_addition;

  // used by: identifiers
  std::shared_ptr<node> m_declaration_node;

  //used by: unary
  int m_unary_indirection_depth;

  // used by: multiple node types
  nodeType m_node_type;
  filePosition m_file_position;

  std::string m_string_value;

};


