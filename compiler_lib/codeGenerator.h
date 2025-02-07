#pragma once
#include <list>
#include "node.h"

class codeGenerator
{
public:
  codeGenerator();
  void setAbstractSyntaxTree(std::list < std::shared_ptr < node > > ast) { m_ast = ast; }
  int startCodeGeneration();
private:

  std::list < std::shared_ptr < node > > m_ast;
  
};

