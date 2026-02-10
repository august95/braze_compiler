#pragma once
#include <list>
#include "../node/node.h"
#include "../node/nodeExpression.h"
#include "../scope.h"
#include "asmWriter.h"
#include "resolver/resolver.h"
#include <unordered_map>
#include <memory>
#include "../braze_compiler.h"


class codeGeneratorExpression
{
public:
  codeGeneratorExpression(asmWriter& asm_writer, resolver& resolver);

  void generateValueNode(std::shared_ptr<nodeExpression> node, std::shared_ptr<resolverEntity> entity);
  void generateExpressionable(std::shared_ptr<nodeExpression> node, int flags);
  void generateExpNode(std::shared_ptr<nodeExpression> node);
  void generateUnary(std::shared_ptr<nodeExpression> node);
  void generateWriteStrings();

private:
  void generateExpressionArithmetic(std::shared_ptr<nodeExpression> node_);
  void generateExpressionLogicalArithmetic(std::shared_ptr<nodeExpression> node);
  bool resolveNodeForValue(std::shared_ptr<nodeExpression> node);
  void generateEntityAccess(std::shared_ptr<nodeExpression> node, std::shared_ptr<resolverResult> result);
  void generateEntityAccessForEntity(std::shared_ptr<resolverEntity> entity, std::shared_ptr<resolverResult> result);
  void generateEntityAccessForFunctionCall(std::shared_ptr<resolverEntity> entity, std::shared_ptr<resolverResult> result);
  void generateEntityAccessForUnaryAddress(std::shared_ptr<resolverEntity> entity, std::shared_ptr<resolverResult> result);
  void generateEntityAccessForUnaryIndirection(std::shared_ptr<resolverEntity> entity, std::shared_ptr<resolverResult> result);
  void generateEntityAccessStart(std::shared_ptr<resolverEntity> root_entity, std::shared_ptr<resolverResult> result);
  void generateAssignmentExpression(std::shared_ptr<nodeExpression> node);
  void generateNumber(std::shared_ptr<nodeExpression> node, int flags);
  void generateIdentifier(std::shared_ptr<nodeExpression> node);
  void generateAssignmentPart(std::shared_ptr<nodeExpression> node, std::string operator_);
  void generateVariableAccess(std::shared_ptr<nodeExpression> node, std::shared_ptr<resolverEntity> entity, int flags);
  void generateMemoryAccess(std::shared_ptr<nodeExpression> node, std::shared_ptr<resolverEntity> entity, int flags);
  void generateAssignmentInstructionForOperator(std::string mov_type, std::string address, std::string reg_to_use, std::string _operator);
  void generateMath(std::string reg1, std::string reg2, ExpressionType exp_type, bool is_signed = false);
  void generateCompare(std::string reg1, std::string reg2);
  void generateString(std::shared_ptr<nodeExpression> node);
  std::string registerString(std::string str);
public:
  int generateLableCount(bool reset = false);
private:
  asmWriter& m_asm_writer;
  resolver& m_resolver;
  std::unordered_map<std::string, std::string> m_strings;
};

