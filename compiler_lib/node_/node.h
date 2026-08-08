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
    NODE_TYPE_UNDEFINED                  = 0x000000000000,
                                         
    NODE_TYPE_EXPRESSION                 = 0x000000000080,
    NODE_TYPE_EXPRESSION_PARANTHESES     = 0x000000000001 | NODE_TYPE_EXPRESSION,
    NODE_TYPE_NUMBER                     = 0x000000000002 | NODE_TYPE_EXPRESSION,
    NODE_TYPE_IDENTIFIER                 = 0x000000000004 | NODE_TYPE_EXPRESSION,
    NODE_TYPE_STRING                     = 0x000000000008 | NODE_TYPE_EXPRESSION,
    NODE_TYPE_UNARY                      = 0x000000000010 | NODE_TYPE_EXPRESSION,
    NODE_TYPE_TENARY                     = 0x000000000020 | NODE_TYPE_EXPRESSION,
                                         
    NODE_TYPE_STATEMENT                  = 0x000000800000,
    NODE_TYPE_STATEMENT_GOTO             = 0x000000000100 | NODE_TYPE_STATEMENT,
    NODE_TYPE_STATEMENT_RETURN           = 0x000000000200 | NODE_TYPE_STATEMENT,
    NODE_TYPE_STATEMENT_IF               = 0x000000000400 | NODE_TYPE_STATEMENT,
    NODE_TYPE_STATEMENT_ELSE             = 0x000000000800 | NODE_TYPE_STATEMENT,
    NODE_TYPE_STATEMENT_WHILE            = 0x000000001000 | NODE_TYPE_STATEMENT,
    NODE_TYPE_STATEMENT_DO_WHILE         = 0x000000002000 | NODE_TYPE_STATEMENT,
    NODE_TYPE_STATEMENT_FOR              = 0x000000004000 | NODE_TYPE_STATEMENT,
    NODE_TYPE_STATEMENT_BREAK            = 0x000000008000 | NODE_TYPE_STATEMENT,
    NODE_TYPE_STATEMENT_CONTINUE         = 0x000000010000 | NODE_TYPE_STATEMENT,
    NODE_TYPE_STATEMENT_SWITCH           = 0x000000020000 | NODE_TYPE_STATEMENT,
    NODE_TYPE_STATEMENT_CASE             = 0x000000040000 | NODE_TYPE_STATEMENT,
    NODE_TYPE_STATEMENT_DEFAULT          = 0x000000080000 | NODE_TYPE_STATEMENT,
                                         
                                         
    NODE_TYPE_VARIABLE_DECLARATION       = 0x000008000000,
    NODE_TYPE_VARIABLE                   = 0x000001000000 | NODE_TYPE_VARIABLE_DECLARATION,
    NODE_TYPE_VARIABLE_LIST              = 0x000002000000 | NODE_TYPE_VARIABLE_DECLARATION,
    NODE_TYPE_BODY                       = 0x000010000000,
    NODE_TYPE_FUNCTION_DECLARATION       = 0x000020000000,
                                         
    //not supported                      
    NODE_TYPE_LIST                       = 0x000200000000,
    NODE_TYPE_LABEL                      = 0x000400000000,
    NODE_TYPE_STRUCT                     = 0x000800000000,
    NODE_TYPE_UNION                      = 0x001000000000,
    NODE_TYPE_BRACKET                    = 0x002000000000,
    NODE_TYPE_CAST                       = 0x004000000000,
    NODE_TYPE_BLANK                      = 0x008000000000,
                                         

    NODE_TYPE_PREPROCESSOR               = 0x8000000000000,
    NODE_TYPE_PREPROCESSOR_NUMBER        = 0x010000000000 | NODE_TYPE_PREPROCESSOR,
    NODE_TYPE_PREPROCESSOR_IDENTIFIER    = 0x020000000000 | NODE_TYPE_PREPROCESSOR,
    NODE_TYPE_PREPROCESSOR_UNARY         = 0x040000000000 | NODE_TYPE_PREPROCESSOR,
    NODE_TYPE_PREPROCESSOR_PARENTHESES   = 0x080000000000 | NODE_TYPE_PREPROCESSOR,
    NODE_TYPE_PREPROCESSOR_EXPRESSION    = 0x100000000000 | NODE_TYPE_PREPROCESSOR,
    NODE_TYPE_PREPROCESSOR_NON_GENERIC   = 0x200000000000 | NODE_TYPE_PREPROCESSOR,
    NODE_TYPE_PREPROCESSOR_KEYWORD       = 0x400000000000 | NODE_TYPE_PREPROCESSOR,
    NODE_TYPE_PREPROCESSOR_JOINED        = 0x800000000000 | NODE_TYPE_PREPROCESSOR,
    NODE_TYPE_PREPROCESSOR_TENARY        = 0x100000000000 | NODE_TYPE_PREPROCESSOR,

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


