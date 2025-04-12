#pragma once
#include <list>
#include "node.h"
#include "scope.h"
#include "asmWriter.h"

class codeGenerator
{
public:
  codeGenerator();
  void setAbstractSyntaxTree(std::list < std::shared_ptr < node > > ast) { m_ast = ast; }
  void initialize();
  void setFileName(std::string filename);
  int startCodeGeneration();
  void generateDataSection();
  void generateRoot();
  void generateRootNode(std::shared_ptr < node > node);
  void generateBody(std::shared_ptr < node > node);
  void generateGlobalVariable(std::shared_ptr < node > node);
  void generateGlobalVariablePrimitive(std::shared_ptr < node > node);
private:

  scope m_root_scope;
  std::string m_input_file;
  std::string output_file;
  std::list < std::shared_ptr < node > > m_ast;
  asmWriter m_asm_writer;

};

