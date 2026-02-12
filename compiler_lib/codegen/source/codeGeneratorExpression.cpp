#include "../../source/pch.h"
#include "../codeGenerator.h"
#include "../codeGeneratorExpression.h"

codeGeneratorExpression::codeGeneratorExpression(asmWriter& asm_writer, resolver& resolver, class codeGenerator* codegen)
  :m_asm_writer(asm_writer),
   m_resolver(resolver),
   m_codegen(codegen)
{
}

void codeGeneratorExpression::generateValueNode(std::shared_ptr<nodeVariableDeclaration> node, std::shared_ptr<resolverEntity> entity)
{
  generateExpressionable(cast_node<nodeExpression>(node->getValueNode()), IS_ASSIGNMENT | IS_RIGHT_HAND_OF_ASSIGNMENT);
  m_asm_writer.asmGenPopIns("eax");
  std::string reg_to_use = "eax";
  std::string mov_type = node->getDatatype()->getDatatypeRegisterSize();
  node->getDatatype()->getRegToUse(reg_to_use);
  generateAssignmentInstructionForOperator(mov_type, entity->getAddress(), reg_to_use, "=");
}

void codeGeneratorExpression::generateExpressionable(std::shared_ptr<nodeExpression> node, int flags)
{
  // flags |= IS_NOT_ROOT;

  if (node->getNodeType() == NODE_TYPE_IDENTIFIER)
  {
    generateIdentifier(node);
  }
  else if (node->getNodeType() == NODE_TYPE_NUMBER)
  {
    // nameless value like '50' without it's own stack offset within the resolver system:  int a = 50;
    assert(node->getDatatype()->isRValue());
    generateNumber(node, flags);
  }
  else if (node->getNodeType() == NODE_TYPE_EXPRESSION)
  {
    generateExpNode(node);
  }
  else if (node->getNodeType() == NODE_TYPE_STRING)
  {
    generateString(node);
  }
  else if (node->getNodeType() == NODE_TYPE_UNARY)
  {
    generateUnary(node);
  }
}

void codeGeneratorExpression::generateExpNode(std::shared_ptr<nodeExpression> node)
{
  if (node->isAssignmentNode())
  {
    generateAssignmentExpression(node);
    return;
  }
  if (resolveNodeForValue(node))
  {
    return;
  }

  generateExpressionArithmetic(node);

  // might be function call
}

void codeGeneratorExpression::generateExpressionArithmetic(std::shared_ptr<nodeExpression> node_)
{
  assert(node_->getNodeType() == NODE_TYPE_EXPRESSION);

  std::shared_ptr<nodeExpression> left = node_->getLeftNode();
  std::shared_ptr<nodeExpression> right = node_->getRightNode();

  if (node_->getExpressionType() & EXPRESSION_LOGICAL_OPERATOR)
  {
    generateExpressionLogicalArithmetic(node_);
  }
  generateExpressionable(left, 0);  // pushes to stack
  generateExpressionable(right, 0); // pushes to stack
  //  std::shared_ptr<datatype> last_datatype_on_stack = right->getDatatype();
  std::shared_ptr<datatype> datatype_on_stack = m_asm_writer.getDatatypeOnStack();
  if (node_->getExpressionType() & EXPRESSION_GEN_MATHABLE)
  {
    std::shared_ptr<datatype> previous_datatype_on_stack = m_asm_writer.getDatatypeOnStack(1);

    m_asm_writer.asmGenPopIns("ecx"); // right node
    m_asm_writer.asmGenPopIns("eax"); // left node

    // FIXME:  operands might be identifiers without datatype. Make the symbol resolver store a pointer to
    // the node that is identified during parsing

    if (datatype_on_stack && previous_datatype_on_stack && (datatype_on_stack->getPointerDepth() > 0 || right->getDatatype()->getPointerDepth() > 0))
    {
      assert(0);
      // handle pointer access
    }

    // result sent is stored in eax
    generateMath("eax", "ecx", node_->getExpressionType());
  }
  m_asm_writer.asmGenPushIns("eax", 0, 0); // we dont always have accces to the assigned node in the node tree from here
}

void codeGeneratorExpression::generateExpressionLogicalArithmetic(std::shared_ptr<nodeExpression> node)
{
  // implement
}

bool codeGeneratorExpression::resolveNodeForValue(std::shared_ptr<nodeExpression> node)
{
  std::shared_ptr<resolverResult> result;
  m_resolver.follow(node, result);
  std::shared_ptr<resolverEntity> entity = result->peekEntity();
  if (!entity)
  {
    return false;
  }
  generateEntityAccess(node, result);
  std::shared_ptr < datatype > datatype = m_asm_writer.getDatatypeOnStack();
  if (datatype->getPointerDepth() != 0)
  {
    //we have a pointer on the stack
    return true;
  }
  m_asm_writer.asmGenPopIns("eax");
  m_asm_writer.asmGenReduceRegister("eax", entity->getDatatype()->getDatatypeSize(), entity->getDatatype()->isSigned());
  m_asm_writer.asmGenPushIns("eax", entity->getDatatype(), 0);

  return true;
}

void codeGeneratorExpression::generateEntityAccess(std::shared_ptr<nodeExpression> node, std::shared_ptr<resolverResult> result)
{
  std::shared_ptr<resolverEntity> root_entity = result->getRootEntity();
  generateEntityAccessStart(root_entity, result);

  while (std::shared_ptr<resolverEntity> entity = result->nextEntity())
  {
    generateEntityAccessForEntity(entity, result);
  }
}

void codeGeneratorExpression::generateEntityAccessForEntity(std::shared_ptr<resolverEntity> entity, std::shared_ptr<resolverResult> result)
{
  if (entity->getEntityType() == E_FUNCTION_CALL)
  {
    generateEntityAccessForFunctionCall(entity, result);
  }
  if (entity->getEntityType() == E_UNARY_ADDRESS)
  {
    generateEntityAccessForUnaryAddress(entity, result);
  }
  if (entity->getEntityType() == E_INDIRECTION)
  {
    generateEntityAccessForUnaryIndirection(entity, result);
  }
}

void codeGeneratorExpression::generateEntityAccessForFunctionCall(std::shared_ptr<resolverEntity> entity, std::shared_ptr<resolverResult> result)
{
  // move function address to ecx
  m_asm_writer.asmGenPopIns("ebx");
  m_asm_writer.asmGen("mov ecx, ebx");
  // FIXME: handle funciton arguments
  std::list<std::shared_ptr<node>> function_arguments = entity->getFunctionArguments();
  for (auto it = function_arguments.rbegin(); it != function_arguments.rend(); ++it)
  {
    std::shared_ptr<node> argument = (*it);
    generateExpressionable(cast_node<nodeExpression>(argument), 0);
  }
  // iterate over arguments and call generateExpressionable() to push variables to stack
  m_asm_writer.asmGen("call ecx");

  m_asm_writer.addStack(entity->getFunctionCallStacksize());
  // m_asm_writer.
  m_asm_writer.asmGenPushIns("eax", entity->getDatatype(), 0);
}

void codeGeneratorExpression::generateEntityAccessForUnaryAddress(std::shared_ptr<resolverEntity> entity, std::shared_ptr<resolverResult> result)
{
  m_asm_writer.asmGenPopIns("ebx");
  m_asm_writer.asmGenPushIns("ebx", entity->getDatatype(), 0);
}

void codeGeneratorExpression::generateEntityAccessForUnaryIndirection(std::shared_ptr<resolverEntity> entity, std::shared_ptr<resolverResult> result)
{
  std::shared_ptr<datatype> datatype = m_asm_writer.getDatatypeOnStack();
  if (!datatype)
    assert(0);
  m_asm_writer.asmGenPopIns("ebx");
  for (int i = 0; i < entity->getUnaryIndirectionDepth(); i++)
  {
    m_asm_writer.asmGen("mov ebx, [ebx]");
  }
  m_asm_writer.asmGenPushIns("ebx", entity->getDatatype(), 0);

}

void codeGeneratorExpression::generateEntityAccessStart(std::shared_ptr<resolverEntity> root_entity, std::shared_ptr<resolverResult> result)
{
  if (root_entity->getCodeGenInstruction() & CG_LOAD_FUNCTION_ADDRESS_TO_EBX)
  {
    m_asm_writer.asmGen("lea ebx, [" + result->getRootAddress() + "]");
    m_asm_writer.asmGenPushIns("ebx", root_entity->getDatatype(), 0);
  }
  else if (root_entity->getCodeGenInstruction() & CG_LOAD_VALUE_TO_EBX)
  {
    m_asm_writer.asmGen("lea ebx, [" + root_entity->getAddress() + "]");
    m_asm_writer.asmGenPushIns("ebx", root_entity->getDatatype(), 0);
    m_asm_writer.getDatatypeOnStack()->incrementPointerDepth();
  }
  else if (root_entity->getEntityType() == E_VARIABLE)
  {
    m_asm_writer.asmGenPushIns("dword [" + root_entity->getAddress() + "]", root_entity->getDatatype(), 0);
  }
}

void codeGeneratorExpression::generateAssignmentExpression(std::shared_ptr<nodeExpression> node)
{
  generateExpressionable(node->getRightNode(), IS_ASSIGNMENT | IS_RIGHT_HAND_OF_ASSIGNMENT);
  // right hand node is now on stack, it can be popped in assignment part
  generateAssignmentPart(node->getLeftNode(), node->getStringValue());
}

void codeGeneratorExpression::generateUnary(std::shared_ptr<nodeExpression> node)
{
  if (resolveNodeForValue(node))
  {
    return;
  }
  else if (STRINGS_EQUAL(node->getStringValue().c_str(), "*"))
  {

  }
  else if (STRINGS_EQUAL(node->getStringValue().c_str(), "&"))
  {

    return;
  }
}

void codeGeneratorExpression::generateNumber(std::shared_ptr<nodeExpression> node, int flags)
{
  // todo add stack verificatoions
  m_asm_writer.asmGenPushIns("dword " + std::to_string(node->getNumberValue()), node->getDatatype(), node->getStackOffset());
}

void codeGeneratorExpression::generateIdentifier(std::shared_ptr<nodeExpression> node)
{
  //always variable, functon calls are expressions with () op
  std::shared_ptr<resolverResult> result;
  m_resolver.follow(node, result);
  std::shared_ptr<resolverEntity> entity = result->peekEntity();
  generateMemoryAccess(node, entity, 0); // push value to stack
}

void codeGeneratorExpression::generateAssignmentPart(std::shared_ptr<nodeExpression> node, std::string operator_)
{
  std::shared_ptr<resolverResult> result;
  m_resolver.follow(node, result);
  std::shared_ptr<resolverEntity> entity = result->peekEntity();
  std::string reg_to_use = "eax";
  std::string mov_type = entity->getNode()->getDatatype()->getDatatypeRegisterSize();
  // fixme: add support for asignment of structs!
  m_asm_writer.asmGenPopIns(reg_to_use);
  entity->getNode()->getDatatype()->getRegToUse(reg_to_use);
  generateAssignmentInstructionForOperator(mov_type, entity->getAddress(), reg_to_use, operator_);
}

void codeGeneratorExpression::generateVariableAccess(std::shared_ptr<nodeExpression> node, std::shared_ptr<resolverEntity> entity, int flags)
{
  generateMemoryAccess(node, entity, flags);
}

void codeGeneratorExpression::generateMemoryAccess(std::shared_ptr<nodeExpression> node, std::shared_ptr<resolverEntity> entity, int flags)
{
  if (flags & GET_ADDRESS)
  {
    // handle pointer access lea instruction
  }

  if (entity->getNode()->getNodeType() == NODE_TYPE_STRUCT)
  {
    // handle struct
  }
  else if (entity->getNode()->getDatatypeSize() != DATA_SIZE_DWORD)
  {
    // handle other sizes than dword
    // alignment in memory is always 4 bytes
    // we need to reduce the number of bytes we are using in the register
    m_asm_writer.asmGen("mov eax, [" + entity->getResolverEntityData()->getAddress() + "]");
    m_asm_writer.asmGenReduceRegister("eax", node->getDatatype()->getDatatypeSize(), node->getDatatype()->isSigned());
    m_asm_writer.asmGenPushIns("eax", node->getDatatype(), node->getStackOffset());
  }
  else if (entity->getNode()->getDatatypeSize() == DATA_SIZE_DWORD)
  {
    // we can push this straight to the stack
    m_asm_writer.asmGenPushIns("dword [" + entity->getResolverEntityData()->getAddress() + "]", node->getDatatype(), node->getStackOffset());
  }
  else
  {
    assert(0);
  }
}

void codeGeneratorExpression::generateAssignmentInstructionForOperator(std::string mov_type, std::string address, std::string reg_to_use, std::string _operator)
{
  if (STRINGS_EQUAL(_operator.c_str(), "="))
  {
    m_asm_writer.asmGen("mov " + mov_type + " [" + address + "], " + reg_to_use);
  }
  else if (STRINGS_EQUAL(_operator.c_str(), "+="))
  {
    m_asm_writer.asmGen("add " + mov_type + " [" + address + "], " + reg_to_use);
  }
}

void codeGeneratorExpression::generateMath(std::string reg1, std::string reg2, ExpressionType exp_type, bool is_signed)
{
  if (exp_type & EXPRESSION_IS_ADDITION)
  {
    m_asm_writer.asmGen("add " + reg1 + ", " + reg2);
  }
  else if (exp_type & EXPRESSION_IS_SUBTRACTION)
  {
    m_asm_writer.asmGen("sub " + reg1 + ", " + reg2);
  }
  else if (exp_type & EXPRESSION_IS_MULTPILICATION)
  {
    m_asm_writer.asmGen("mov ecx, " + reg2);
    if (is_signed)
    {
      m_asm_writer.asmGen("imul ecx");
    }
    else
    {
      m_asm_writer.asmGen("mul ecx");
    }
  }
  else if (exp_type & EXPRESSION_IS_DIVISION)
  {
    m_asm_writer.asmGen("mov ecx, " + reg2);
    m_asm_writer.asmGen("cdq");
    if (is_signed)
    {
      m_asm_writer.asmGen("idiv ecx");
    }
    else
    {
      m_asm_writer.asmGen("div ecx");
    }
  }
  else if (exp_type & EXPRESSION_IS_MODULUS)
  {
    m_asm_writer.asmGen("mov ecx, " + reg2);
    m_asm_writer.asmGen("cdq");
    if (is_signed)
    {
      m_asm_writer.asmGen("idiv ecx");
    }
    else
    {
      m_asm_writer.asmGen("div ecx");
    }

    m_asm_writer.asmGen("mov eax, edx");
  }
  else if (exp_type & EXPRESSION_IS_ABOVE)
  {
    generateCompare(reg2, "setg");
  }
  else if (exp_type & EXPRESSION_IS_BELOW)
  {
    generateCompare(reg2, "setl");
  }
  else if (exp_type & EXPRESSION_IS_EQUAL)
  {
    generateCompare(reg2, "sete");
  }
  else if (exp_type & EXPRESSION_IS_ABOVE_OR_EQUAL)
  {
    generateCompare(reg2, "setge");
  }
  else if (exp_type & EXPRESSION_IS_BELOW_OR_EQUAL)
  {
    generateCompare(reg2, "setle");
  }
  else if (exp_type & EXPRESSION_IS_NOT_EQUAL)
  {
    generateCompare(reg2, "setne");
  }
  /*
  else if (exp_type & EXPRESSION_BITSHIFT_LEFT)
  {
    reg2 = subRegister(reg2, DATA_SIZE_BYTE);
    m_asm_writer.asmPush("sal " + reg1 + ", " + reg2);
  }
  else if (exp_type & EXPRESSION_BITSHIFT_RIGHT)
  {
    reg2 = subRegister(reg2, DATA_SIZE_BYTE);
    m_asm_writer.asmPush("sar " + reg1 + ", " + reg2);
  }
  */
  else if (exp_type & EXPRESSION_IS_BITWISE_AND)
  {
    m_asm_writer.asmGen("and " + reg1 + ", " + reg2);
  }
  else if (exp_type & EXPRESSION_IS_BITWISE_OR)
  {
    m_asm_writer.asmGen("or " + reg1 + ", " + reg2);
  }
  else if (exp_type & EXPRESSION_IS_BITWISE_XOR)
  {
    m_asm_writer.asmGen("xor " + reg1 + ", " + reg2);
  }
}

void codeGeneratorExpression::generateCompare(std::string reg1, std::string reg2)
{
  m_asm_writer.asmGen("cmp eax, " + reg1);
  m_asm_writer.asmGen(reg2 + " al");
  m_asm_writer.asmGen("movzx eax, al");
}

void codeGeneratorExpression::generateWriteStrings()
{
  for (const auto& pair : m_strings)
  {
    std::string str = pair.first;
    m_asm_writer.asmGenNoNewLine(pair.second + ": db ");
    for (int i = 0; i < str.size(); i++)
    {
      char char_[10];
      std::snprintf(char_, sizeof(char_), "'%c', ", str.c_str()[i]);
      m_asm_writer.asmGenNoNewLine(char_);
    }
    m_asm_writer.asmGenNoNewLine("0");
    m_asm_writer.asmGen(""); // new line
  }
}

void codeGeneratorExpression::generateString(std::shared_ptr<nodeExpression> node)
{
  std::string label = registerString(node->getStringValue());
  m_asm_writer.asmGen("mov eax, " + label);
  m_asm_writer.asmGenPushIns("eax", node->getDatatype(), 0);
}

std::string codeGeneratorExpression::registerString(std::string str)
{
  if (m_strings.find(str) != m_strings.end())
  {
    return m_strings[str];
  }
  m_strings[str] = "str_" + std::to_string(m_codegen->generateLableCount());
  return m_strings[str];
}

