#pragma once
#include "node.h"


class nodeBody :
    public node
{
public:
  nodeBody();
  nodeBody(nodeType node_type, filePosition file_position);

  void calculateStackOffset(int& stack_offset) override;

  void addStatement(std::shared_ptr<node> statement);
  //  void setStatements(std::list < std::shared_ptr < node > > statements);
  std::list<std::shared_ptr<node>> getStatements() { return m_statements; }

  int getBodySize() { return m_body_size; }
  virtual void setStackOffset(int stack_offset) { m_stack_offset = stack_offset; }
  int getStackOffset() { return m_stack_offset; }


private:
  // used by: variable nodes and body nodes
  int m_stack_offset;

  // used by: body nodes
  std::list<std::shared_ptr<node>> m_statements;
  int m_body_size;

};

