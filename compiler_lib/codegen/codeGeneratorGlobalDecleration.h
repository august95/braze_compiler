#pragma once
#include <unordered_map>
#include <memory>
#include "../braze_compiler.h"
#include "../node/node.h"
#include "../node/nodeBody.h"
#include "../node/nodeFunctionDeclaration.h"
#include "../node/nodeVariableDeclaration.h"
#include "../scope.h"
#include "asmWriter.h"
#include "resolver/resolver.h"

//handles declarations of global functins, global variables and variables

class codeGeneratorGlobalDecleration
{
public:
  codeGeneratorGlobalDecleration(asmWriter& asm_writer, resolver& resolver, class codeGeneratorStatement* codegen_statement, class codeGeneratorExpression* codegen_expression, class codeGenerator* codegen);
  void generateGlobalVariable(std::shared_ptr<nodeVariableDeclaration> node);
  void generateFunction(std::shared_ptr<nodeFunctionDeclaration> node);
  void generateBody(std::shared_ptr<nodeBody> node); //todo: move

private:
  void generateFunctionDeclaration(std::shared_ptr<nodeFunctionDeclaration> node);
  void generateFunctionParameters(std::shared_ptr<nodeFunctionDeclaration> node_);
  void generateScope(std::shared_ptr<nodeBody> node);
  void generateGlobalVariablePrimitive(std::shared_ptr<nodeVariableDeclaration> node);

private:
  class codeGenerator* m_codegen;
  class codeGeneratorStatement* m_codegen_statement;
  class codeGeneratorExpression* m_codegen_expression;
  asmWriter& m_asm_writer;
  resolver& m_resolver;
};

