// add headers that you want to pre-compile here
#pragma once

#include "framework.h"
#include <iostream>
#include <assert.h>
#include "node/node.h"
#include "node/nodeExpression.h"
#include "node/nodeStatement.h"
#include "node/nodeVariableDeclaration.h"

#include "filePosition.h"
#include "token.h"

enum
{
  IS_ASSIGNMENT = 0x1,
  IS_RIGHT_HAND_OF_ASSIGNMENT = 0x2,
  IS_NOT_ROOT = 0x4,
  GET_ADDRESS = 0x8
};



// TODO: add contex for where the error message was called. PARSER, LEXER, filename, line, col
static void cerror(const char *error)
{
  std::cout << "Compiler Error: " << error << "\n";
}

static void cwarning(const char *warning)
{
  std::cout << "Compiler Warning: " << warning << "\n";
}

static void clog(const char *log)
{
  std::cout << "Log: " << log << "\n";
}

static void cerror(const char *error, filePosition file_position)
{
  std::cout << "Compiler Error: " << error << "  in " << file_position.getLocationString().c_str() << "\n";
}

static void cwarning(const char *warning, filePosition file_position)
{
  std::cout << "Compiler Warning: " << warning << "  in " << file_position.getLocationString().c_str() << "\n";
}

static void clog(const char *log, filePosition file_position)
{
  std::cout << "Log: " << log << "  in " << file_position.getLocationString().c_str() << "\n";
}


template <class t>
bool is_valid(nodeType node_type)
{
  if (node_type | NODE_TYPE_EXPRESSION)
  {
    return std::is_same_v<t, nodeExpression>;
  }
  else if (node_type | NODE_TYPE_STATEMENT)
  {
    return std::is_same_v<t, nodeStatement>;
  }
  else if (node_type | NODE_TYPE_VARIABLE_DECLARATION)
  {
    return std::is_same_v<t, nodeVariableDeclaration>;
  }
  return false;
}

template <class nodeType>
std::shared_ptr<nodeType> cast_node(std::shared_ptr<node> node_)
{
  // Use std::static_pointer_cast<nodeType> to cast from base class to derived class

  if (!node_)
    return std::shared_ptr<nodeType>();
  /*
  if (!is_valid<nodeType>(node_->getNodeType()))
  {
    assert(0, "invalid node type conversion");
  }
  */
  std::shared_ptr<nodeType> cast_node_ = std::static_pointer_cast<nodeType>(node_);
  return cast_node_;
}
