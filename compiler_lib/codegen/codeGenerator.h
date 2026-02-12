#pragma once
#include <list>
#include "../node/node.h"
#include "../node/nodeExpression.h"
#include "../node/nodeStatement.h"
#include "../node/nodeBody.h"
#include "../node/nodeVariableDeclaration.h"
#include "../node/nodeFunctionDeclaration.h"
#include "../scope.h"
#include "asmWriter.h"
#include "resolver/resolver.h"
#include <unordered_map>
#include <memory>
#include "codeGeneratorStatement.h"
#include "codeGeneratorExpression.h"
#include "codeGeneratorGlobalDecleration.h"
#include "../braze_compiler.h"


/*
 * todo
 * pointers
 * finish operators
 * logical operators
 * check that unsigned logic is correct
 * return break continue
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
  int generateLableCount(bool reset = false);

private:
  scope m_root_scope;
  std::string m_input_file;
  std::string output_file;
  std::list<std::shared_ptr<node>> m_ast;
  asmWriter m_asm_writer;
  resolver m_resolver;

  codeGeneratorExpression m_codegen_expression;
  codeGeneratorStatement m_codegen_statement;
  codeGeneratorGlobalDecleration m_codegen_global_declaration;
};
