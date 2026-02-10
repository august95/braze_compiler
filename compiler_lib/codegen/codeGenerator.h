#pragma once
#include <list>
#include "../node/node.h"
#include "../scope.h"
#include "asmWriter.h"
#include "resolver/resolver.h"
#include <unordered_map>
#include <memory>
#include "codeGeneratorExpression.h"
#include "../braze_compiler.h"


/*
 * todo
 * pointers
 * finish operators
 * logical operators
 * check that unsigned logic is correct
 * control flow parsing & generation
 *
 */
class codeGenerator
{
public:
  codeGenerator();
  void setAbstractSyntaxTree(std::list<std::shared_ptr<node>> ast) { m_ast = ast; }
  void initialize();
  void setFileName(std::string filenam, asmWriter::WriteMode write_mode = asmWriter::WriteMode::W_FILE);
  int startCodeGeneration();
  void generateDataSection();
  void generateRoot();
  void generateReadOnlySection();
  void generateRootNode(std::shared_ptr<node> node);
  void generateFunction(std::shared_ptr<node> node);
  void generateFunctionParameters(std::shared_ptr<node> node_);
  void generateBody(std::shared_ptr<node> node);
  void generateScope(std::shared_ptr<node> node);
  void generateStatement(std::shared_ptr<node> node);
  void generateStatementFor(std::shared_ptr<node> node);
  void generateStatementWhile(std::shared_ptr<node> node);
  void generateStatementIf(std::shared_ptr<node> node);
  void generateStatementIf_(std::shared_ptr<node> node, int end_label);
  void generateStatementIfElse(std::shared_ptr<node> node, int end_label);
  void generateStatementElse(std::shared_ptr<node> node, int end_label);
  void generateGlobalVariable(std::shared_ptr<node> node);
  void generateGlobalVariablePrimitive(std::shared_ptr<node> node);
  void generateScopedVariable(std::shared_ptr<node> node);
  int generateLableCount(bool reset = false);




private:
  scope m_root_scope;
  std::string m_input_file;
  std::string output_file;
  std::list<std::shared_ptr<node>> m_ast;
  asmWriter m_asm_writer;
  resolver m_resolver;

  codeGeneratorExpression m_codegen_expression;
};
