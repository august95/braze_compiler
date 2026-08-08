#pragma once
// node created by the parser,  is leafs in the abstract syntax tree

#include <string>
#include "../filePosition.h"
#include "../datatype.h"
#include <iostream>
#include <list>
#include <memory>
#include <cstdint>

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
  
  NODE_TYPE_VARIABLE_DECLARATION    = 0x0008000000,
  NODE_TYPE_VARIABLE                = 0x0001000000 | NODE_TYPE_VARIABLE_DECLARATION,
  NODE_TYPE_VARIABLE_LIST           = 0x0002000000 | NODE_TYPE_VARIABLE_DECLARATION,
  NODE_TYPE_BODY                    = 0x0010000000,
  NODE_TYPE_FUNCTION_DECLARATION    = 0x0020000000,
  
  //not supported
  NODE_TYPE_LIST                    = 0x0200000000,
  NODE_TYPE_LABEL                   = 0x0400000000,
  NODE_TYPE_STRUCT                  = 0x0800000000,
  NODE_TYPE_UNION                   = 0x1000000000,
  NODE_TYPE_BRACKET                 = 0x2000000000,
  NODE_TYPE_CAST                    = 0x4000000000,
  NODE_TYPE_BLANK                   = 0x8000000000
  NODE_TYPE_PREPROCESSOR               = 0x8000000000000,

};


class node
{
public:
  node();
  node(nodeType node_type, filePosition file_position);

  filePosition getFilePosition() { return m_file_position; }
  nodeType getNodeType() { return m_node_type; }
  virtual void calculateStackOffset(int& stack_offset) = 0;

  //create declaration interface
  virtual std::shared_ptr<datatype> getDatatype() { return 0; }
  virtual void setIsGlobal(bool is_global) {  }
  virtual int getDatatypeSize() { return 0; };
  //end declaration interface

  virtual void setStringValue(std::string string_value) { m_string_value = string_value; } //FIXME resolve dtype owership with other datatype owners
  std::string getStringValue() { return m_string_value; }

protected:


  nodeType m_node_type;
  filePosition m_file_position;
  std::string m_string_value;

};


