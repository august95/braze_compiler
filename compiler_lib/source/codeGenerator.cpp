#include "pch.h"
#include "../codeGenerator.h"
#include "../braze_compiler.h"
#include "../resolverResult.h"

#define C_STACK_ALIGNMENT 16
#define STACK_PUSH_SIZE 4
#define C_ALIGN(size) (size % C_STACK_ALIGNMENT) ? size + (C_STACK_ALIGNMENT - (size % C_STACK_ALIGNMENT)) : size

codeGenerator::codeGenerator()
{
}

void codeGenerator::initialize()
{
  int error = m_asm_writer.initialize(output_file);

  m_resolver.initialize();

  if (error != 0)
  {
    cerror("failed to open file");
  }
}

void codeGenerator::setFileName(std::string filename, asmWriter::WriteMode write_mode)
{
  m_input_file = filename;
  output_file = filename + ".asm";
  m_asm_writer.setWriteMode(write_mode);
}

int codeGenerator::startCodeGeneration()
{

  initialize();

  m_root_scope.init(std::make_shared<scope>(), true);

  generateDataSection();

  generateRoot();

  generateReadOnlySection();

  m_asm_writer.close();

  return 0;
}

void codeGenerator::generateDataSection()
{
  m_asm_writer.asmGen("section .data");

  for (auto node : m_ast)
  {
    if (node->getNodeType() == NODE_TYPE_VARIABLE)
    {
      generateGlobalVariable(node);
    }
  }
}

void codeGenerator::generateRoot()
{
  m_asm_writer.asmGen("section .text");

  for (auto node : m_ast)
  {
    if (node->getNodeType() == NODE_TYPE_VARIABLE)
    {
      // Global variables already processed in generateDataSection
    }
    if (node->getNodeType() == NODE_TYPE_FUNCTION)
    {
      generateFunction(node);
    }
  }
}

void codeGenerator::generateReadOnlySection()
{
  m_asm_writer.asmGen("section .rodata");
  generateWriteStrings();
}

void codeGenerator::generateRootNode(std::shared_ptr<node> node)
{
  if (node->getNodeType() == NODE_TYPE_FUNCTION)
  {
    m_resolver.registerFunction(node);
    if (node->isFunctionPrototype())
    {
      m_asm_writer.asmGen("extern " + node->getStringValue());
    }
    else
    {
      // function declaration
      std::string function_name = node->getStringValue();
      m_asm_writer.asmGen("global " + function_name);
      m_asm_writer.asmGen(function_name + ":");

      bool has_stack_size = C_ALIGN(node->getBodyNode()->getBodySize()) != 0;
      m_asm_writer.asmGenPushEbp(C_ALIGN(node->getBodyNode()->getBodySize()));

      m_resolver.createNewScope(true, false);
      generateFunctionParameters(node);

      m_resolver.createNewScope(true, false);
      generateBody(node->getBodyNode());
      m_resolver.removeScope();

      m_resolver.removeScope();
      m_asm_writer.asmGenPopEbp(C_ALIGN(node->getBodyNode()->getBodySize()));
    }
  }
}

void codeGenerator::generateFunction(std::shared_ptr<node> node)
{
  // deal with forward declaration
  generateRootNode(node);
}

void codeGenerator::generateFunctionParameters(std::shared_ptr<node> node_)
{
  std::list<std::shared_ptr<node>> function_arguemnt = node_->getFunctionArguments();
  for (auto it = function_arguemnt.begin(); it != function_arguemnt.end(); ++it)
  {
    std::shared_ptr<node> argument_node = (*it);
    std::shared_ptr<resolverEntity> entity = m_resolver.addEntity(argument_node, true);
  }
}

void codeGenerator::generateBody(std::shared_ptr<node> node)
{
  m_resolver.createNewScope(false, true);
  generateScope(node);
  m_resolver.removeScope();
}

void codeGenerator::generateScope(std::shared_ptr<node> node)
{
  if (node)
  {
    if (node->getNodeType() == NODE_TYPE_BODY)
    {
      for (auto statement : node->getStatements())
      {
        generateStatement(statement);
      }
    }
  }
}

void codeGenerator::generateStatement(std::shared_ptr<node> node)
{

  if (node->getNodeType() == NODE_TYPE_VARIABLE)
  {
    generateScopedVariable(node);
  }
  else if (node->getNodeType() == NODE_TYPE_EXPRESSION)
  {
    generateExpNode(node);
  }
  else if (node->getNodeType() == NODE_TYPE_STATEMENT_IF)
  {
    generateStatementIf(node);
  }
  else if (node->getNodeType() == NODE_TYPE_STATEMENT_WHILE)
  {
    generateStatementWhile(node);
  }
  else if (node->getNodeType() == NODE_TYPE_STATEMENT_FOR)
  {
    generateStatementFor(node);
  }
  else if (node->getNodeType() == NODE_TYPE_UNARY)
  {
    generateUnary(node);
  }
  m_asm_writer.discardUnusedStack();
}

void codeGenerator::generateStatementFor(std::shared_ptr<node> node)
{
  int for_loop_start = generateLableCount();
  int for_loop_end = generateLableCount();
  if (node->getInitNode())
  {
    generateScopedVariable(node->getInitNode());
    // m_asm_writer.asmGenPopIns("eax");
  }

  m_asm_writer.asmGen("jmp .for_loop" + std::to_string(for_loop_start));
  if (node->getLoopNode())
  {
    generateExpressionable(node->getLoopNode(), 0);
    // m_asm_writer.asmGenPopIns("eax");
  }
  m_asm_writer.asmGen(".for_loop" + std::to_string(for_loop_start) + ":");
  if (node->getConditionNode())
  {
    generateExpressionable(node->getConditionNode(), 0);
    m_asm_writer.asmGenPopIns("eax");
    m_asm_writer.asmGen("cmp eax, 0");
    m_asm_writer.asmGen("je .for_loop_end" + std::to_string(for_loop_end));
  }

  if (node->getBodyNode())
  {
    generateBody(node->getBodyNode());
  }

  if (node->getLoopNode())
  {
    generateExpressionable(node->getLoopNode(), 0);
    // m_asm_writer.asmGenPopIns("eax");
  }

  m_asm_writer.asmGen("jmp .for_loop" + std::to_string(for_loop_start));
  m_asm_writer.asmGen(".for_loop_end" + std::to_string(for_loop_end) + ":");
}

void codeGenerator::generateStatementWhile(std::shared_ptr<node> node)
{
  int while_start = generateLableCount();
  int while_end = generateLableCount();

  m_asm_writer.asmGen(".while_start_" + std::to_string(while_start) + ":");
  generateExpressionable(node->getConditionNode(), 0);
  m_asm_writer.asmGenPopIns("eax");
  m_asm_writer.asmGen("cmp eax, 0");
  m_asm_writer.asmGen("je .while_end_" + std::to_string(while_end));
  generateBody(node->getBodyNode());
  m_asm_writer.asmGen("jmp .while_start_" + std::to_string(while_start));
  m_asm_writer.asmGen(".while_end_" + std::to_string(while_end) + ":");
}

void codeGenerator::generateStatementIf(std::shared_ptr<node> node)
{
  int end_label = generateLableCount();
  generateStatementIf_(node, end_label);

  m_asm_writer.asmGen(".if_end_" + std::to_string(end_label) + ":");
}

void codeGenerator::generateStatementIf_(std::shared_ptr<node> node, int end_label)
{
  int if_label = generateLableCount();

  generateExpressionable(node->getConditionNode(), 0);
  m_asm_writer.asmGenPopIns("eax");
  m_asm_writer.asmGen("cmp eax, 0");                         // if equal, sets zero flag in CPU
  m_asm_writer.asmGen("je .if_" + std::to_string(if_label)); // if zero flag is set in CPU, perfores jump. We dont want to jump when if(0)
  generateBody(node->getBodyNode());
  m_asm_writer.asmGen("jmp .if_end_" + std::to_string(end_label));
  m_asm_writer.asmGen(".if_" + std::to_string(if_label) + ":");

  if (node->getNextElseNode())
  {
    generateStatementIfElse(node->getNextElseNode(), end_label);
  }
}

void codeGenerator::generateStatementIfElse(std::shared_ptr<node> node, int end_label)
{
  if (node->getNodeType() == NODE_TYPE_STATEMENT_IF)
  {
    generateStatementIf_(node, end_label);
  }
  else if (node->getNodeType() == NODE_TYPE_STATEMENT_ELSE)
  {
    generateStatementElse(node, end_label);
  }
  else
  {
    assert(0);
  }
}

void codeGenerator::generateStatementElse(std::shared_ptr<node> node, int end_label)
{
  generateBody(node->getBodyNode());
}

void codeGenerator::generateGlobalVariable(std::shared_ptr<node> node)
{
  if (node)
  {
    if (node->getNodeType() == NODE_TYPE_VARIABLE)
    {
      std::shared_ptr<datatype> datatype = node->getDatatype();

      // do not deal with structs and unions here!
      if (datatype->isPrimitiveType())
      {
        generateGlobalVariablePrimitive(node);
      }
    }
  }
}

void codeGenerator::generateGlobalVariablePrimitive(std::shared_ptr<node> node)
{
  if (node->getNodeType() == NODE_TYPE_STRING)
  {
    cerror("Codegen: strings not yet implemented");
    return;
  }
  std::shared_ptr<resolverEntity> entity = m_resolver.addEntity(node, false);

  std::string var_name = node->getStringValue();
  std::string var_value = "0";
  std::shared_ptr<datatype> datatype = node->getDatatype();
  if (node->getValueNode())
  {
    var_value = std::to_string(node->getValueNode()->getNumberValue());
  }
  m_asm_writer.asmGen(var_name + ": " + datatype->getStringForPrimitiveSize() + " " + var_value);
}

void codeGenerator::generateScopedVariable(std::shared_ptr<node> node)
{
  if (node->getNodeType() == NODE_TYPE_VARIABLE)
  {
    std::shared_ptr<resolverEntity> entity = m_resolver.addEntity(node, true);
    if (node->getValueNode())
    {
      generateExpressionable(node->getValueNode(), IS_ASSIGNMENT | IS_RIGHT_HAND_OF_ASSIGNMENT);
      m_asm_writer.asmGenPopIns("eax");
      std::string reg_to_use = "eax";
      std::string mov_type = node->getDatatype()->getDatatypeRegisterSize();
      node->getDatatype()->getRegToUse(reg_to_use);
      generateAssignmentInstructionForOperator(mov_type, entity->getAddress(), reg_to_use, "=");
    }
  }
}

void codeGenerator::generateExpressionable(std::shared_ptr<node> node, int flags)
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

void codeGenerator::generateExpNode(std::shared_ptr<node> node)
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

void codeGenerator::generateExpressionArithmetic(std::shared_ptr<node> node_)
{
  assert(node_->getNodeType() == NODE_TYPE_EXPRESSION);

  std::shared_ptr<node> left = node_->getLeftNode();
  std::shared_ptr<node> right = node_->getRightNode();

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

void codeGenerator::generateExpressionLogicalArithmetic(std::shared_ptr<node> node)
{
  // implement
}

bool codeGenerator::resolveNodeForValue(std::shared_ptr<node> node)
{
  std::shared_ptr<resolverResult> result;
  m_resolver.follow(node, result);
  std::shared_ptr<resolverEntity> entity = result->peekEntity();
  if (!entity)
  {
    return false;
  }
  generateEntityAccess(node, result);
  std::shared_ptr < datatype > datatype =  m_asm_writer.getDatatypeOnStack();
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

void codeGenerator::generateEntityAccess(std::shared_ptr<node> node, std::shared_ptr<resolverResult> result)
{
  std::shared_ptr<resolverEntity> root_entity = result->getRootEntity();
  generateEntityAccessStart(root_entity, result);

  while (std::shared_ptr<resolverEntity> entity = result->nextEntity())
  {
    generateEntityAccessForEntity(entity, result);
  }
}

void codeGenerator::generateEntityAccessForEntity(std::shared_ptr<resolverEntity> entity, std::shared_ptr<resolverResult> result)
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

void codeGenerator::generateEntityAccessForFunctionCall(std::shared_ptr<resolverEntity> entity, std::shared_ptr<resolverResult> result)
{
  // move function address to ecx
  m_asm_writer.asmGenPopIns("ebx");
  m_asm_writer.asmGen("mov ecx, ebx");
  // FIXME: handle funciton arguments
  std::list<std::shared_ptr<node>> function_arguments = entity->getFunctionArguments();
  for (auto it = function_arguments.rbegin(); it != function_arguments.rend(); ++it)
  {
    std::shared_ptr<node> argument = (*it);
    generateExpressionable(argument, 0);
  }
  // iterate over arguments and call generateExpressionable() to push variables to stack
  m_asm_writer.asmGen("call ecx");

  m_asm_writer.addStack(entity->getFunctionCallStacksize());
  // m_asm_writer.
  m_asm_writer.asmGenPushIns("eax", entity->getDatatype(), 0);
}

void codeGenerator::generateEntityAccessForUnaryAddress(std::shared_ptr<resolverEntity> entity, std::shared_ptr<resolverResult> result)
{
  m_asm_writer.asmGenPopIns("ebx");
  m_asm_writer.asmGenPushIns("ebx", entity->getDatatype(), 0);
}

void codeGenerator::generateEntityAccessForUnaryIndirection(std::shared_ptr<resolverEntity> entity, std::shared_ptr<resolverResult> result)
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

void codeGenerator::generateEntityAccessStart(std::shared_ptr<resolverEntity> root_entity, std::shared_ptr<resolverResult> result)
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

void codeGenerator::generateAssignmentExpression(std::shared_ptr<node> node)
{
  generateExpressionable(node->getRightNode(), IS_ASSIGNMENT | IS_RIGHT_HAND_OF_ASSIGNMENT);
  // right hand node is now on stack, it can be popped in assignment part
  generateAssignmentPart(node->getLeftNode(), node->getStringValue());
}

void codeGenerator::generateUnary(std::shared_ptr<node> node)
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

void codeGenerator::generateNumber(std::shared_ptr<node> node, int flags)
{
  // todo add stack verificatoions
  m_asm_writer.asmGenPushIns("dword " + std::to_string(node->getNumberValue()), node->getDatatype(), node->getStackOffset());
}

void codeGenerator::generateIdentifier(std::shared_ptr<node> node)
{
  //always variable, functon calls are expressions with () op
  std::shared_ptr<resolverResult> result;
  m_resolver.follow(node, result);
  std::shared_ptr<resolverEntity> entity = result->peekEntity();
  generateMemoryAccess(node, entity, 0); // push value to stack
}

void codeGenerator::generateAssignmentPart(std::shared_ptr<node> node, std::string operator_)
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

void codeGenerator::generateVariableAccess(std::shared_ptr<node> node, std::shared_ptr<resolverEntity> entity, int flags)
{
  generateMemoryAccess(node, entity, flags);
}

void codeGenerator::generateMemoryAccess(std::shared_ptr<node> node, std::shared_ptr<resolverEntity> entity, int flags)
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

void codeGenerator::generateAssignmentInstructionForOperator(std::string mov_type, std::string address, std::string reg_to_use, std::string _operator)
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

void codeGenerator::generateMath(std::string reg1, std::string reg2, ExpressionType exp_type, bool is_signed)
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

void codeGenerator::generateCompare(std::string reg1, std::string reg2)
{
  m_asm_writer.asmGen("cmp eax, " + reg1);
  m_asm_writer.asmGen(reg2 + " al");
  m_asm_writer.asmGen("movzx eax, al");
}

void codeGenerator::generateWriteStrings()
{
  for (const auto &pair : m_strings)
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

void codeGenerator::generateString(std::shared_ptr<node> node)
{
  std::string label = registerString(node->getStringValue());
  m_asm_writer.asmGen("mov eax, " + label);
  m_asm_writer.asmGenPushIns("eax", node->getDatatype(), 0);
}

std::string codeGenerator::registerString(std::string str)
{
  if (m_strings.find(str) != m_strings.end())
  {
    return m_strings[str];
  }
  m_strings[str] = "str_" + std::to_string(generateLableCount());
  return m_strings[str];
}

int codeGenerator::generateLableCount(bool reset)
{
  static int count = 0;
  count++;

  // static counter lives during all unit tests, needs to be cleared after each tests
  if (reset)
  {
    count = 0;
  }
  return count;
}
