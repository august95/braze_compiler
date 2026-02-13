#pragma once

#include <unordered_map>
#include <memory>
#include "../braze_compiler.h"
#include "../node_/node.h"
#include "../node_/nodeStatement.h"
#include "../scope.h"
#include "asmWriter.h"
#include "resolver/resolver.h"

class codeGenerator;
class codeGeneratorExpression;
class codeGeneratorStatement
{
public:
  codeGeneratorStatement(asmWriter& asm_writer, resolver& resolver, class codeGeneratorExpression* codegen_expression,class codeGeneratorGlobalDecleration* codegen_global_declaration, class codeGenerator* codegen);

  void generateStatement(std::shared_ptr<node> node);

private:
  void generateStatementFor(std::shared_ptr<nodeStatement> node);
  void generateStatementWhile(std::shared_ptr<nodeStatement> node);
  void generateStatementIf(std::shared_ptr<nodeStatement> node);
  void generateStatementIf_(std::shared_ptr<nodeStatement> node, int end_label);
  void generateStatementIfElse(std::shared_ptr<nodeStatement> node, int end_label);
  void generateStatementElse(std::shared_ptr<nodeStatement> node, int end_label);
  void generateScopedVariable(std::shared_ptr<nodeVariableDeclaration> node); //todo move to statement

private:
  class codeGenerator* m_codegen;
  class codeGeneratorExpression* m_codegen_expression;
  class codeGeneratorGlobalDecleration* m_codegen_global_declaration;
  asmWriter& m_asm_writer;
  resolver& m_resolver;
};

