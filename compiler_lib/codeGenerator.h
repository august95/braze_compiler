#pragma once
#include <list>
#include "node.h"
#include "scope.h"
#include "asmWriter.h"
#include "resolver.h"

enum
{
  IS_ASSIGNMENT = 0x1,
  IS_RIGHT_HAND_OF_ASSIGNMENT = 0x2,
  IS_NOT_ROOT = 0x4,
  GET_ADDRESS = 0x8
};

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
  void generateFunction(std::shared_ptr < node > node);
  void generateBody(std::shared_ptr < node > node);
  void generateScope(std::shared_ptr<node> node);
  void generateStatement(std::shared_ptr<node> node);
  void generateGlobalVariable(std::shared_ptr < node > node);
  void generateGlobalVariablePrimitive(std::shared_ptr < node > node);
  void generateScopedVariable(std::shared_ptr < node > node);
  void generateExpressionable(std::shared_ptr < node > node, int flags);
  void generateExpNode(std::shared_ptr < node > node);
  bool resolveNodeForValue(std::shared_ptr < node > node);
  void generateAssignmentExpression(std::shared_ptr < node > node);
  void generateNumber(std::shared_ptr<node> node, int flags);
  void generateIdentifier(std::shared_ptr<node> node);
  void generateAssignmentPart(std::shared_ptr < node > node, std::string operator_);
  void generateVariableAccess(std::shared_ptr < node > node, std::shared_ptr<resolverEntity> entity, int flags);
  void generateMemoryAccess(std::shared_ptr < node > node, std::shared_ptr<resolverEntity> entity, int flags);
  void generateAssignmentInstructionForOperator(std::string mov_type, std::string address, std::string reg_to_use, std::string _operator);
private:

  scope m_root_scope;
  std::string m_input_file;
  std::string output_file;
  std::list < std::shared_ptr < node > > m_ast;
  asmWriter m_asm_writer;
  resolver m_resolver;


};

