#pragma once
#include "node.h"
#include "nodeBody.h"


class nodeFunctionDeclaration :
    public node
{
public:
  nodeFunctionDeclaration();
  nodeFunctionDeclaration(nodeType node_type, filePosition file_position);

  virtual void calculateStackOffset(int& stack_offset) override;

  void setBodyNode(std::shared_ptr<nodeBody> body_node) { m_body_node = body_node; }
  std::shared_ptr<nodeBody> getBodyNode() { return m_body_node; }
  void setReturnDatatype(std::shared_ptr<datatype> dtype) { m_return_datatype = dtype; }
  std::shared_ptr<datatype> getReturnDatatype() { return m_return_datatype; }

  void addFunctionArgumentNode(std::shared_ptr<node> node) { m_function_arguemnt.push_back(node); }
  std::list<std::shared_ptr<node>> getFunctionArguments() { return m_function_arguemnt; }


  bool isFunctionPrototype() { return m_function_prototype; }
  void setFunctionPrototype(bool function_prototype) { m_function_prototype = function_prototype; }

private:
  std::shared_ptr<nodeBody> m_body_node;
  std::shared_ptr<datatype> m_return_datatype;
  std::list<std::shared_ptr<node>> m_function_arguemnt;

  int m_stack_size;
  bool m_function_prototype;
  // size of ebp and esp, migth be bigger when returning structs
  int m_stack_addition;
};

