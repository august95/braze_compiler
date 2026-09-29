#pragma once
#include "node.h"
#include "nodeVariableDeclaration.h"
#include <list>
#include <memory>

class nodeStructDeclaration : public node
{
public:
  nodeStructDeclaration(std::string name, filePosition file_position);

  void addMember(std::shared_ptr<nodeVariableDeclaration> member);
  std::shared_ptr<nodeVariableDeclaration> findMember(const std::string& name);
  int getStructSize() const { return m_size + datatype::Padding(m_size, m_alignment); }
  int getStructAlignment() const { return m_alignment; }

  virtual void calculateStackOffset(int& stack_offset) override {}

private:
  std::list<std::shared_ptr<nodeVariableDeclaration>> m_members;
  int m_size;
  int m_alignment;
};
