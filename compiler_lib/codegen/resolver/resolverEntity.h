#pragma once
#include <string>
#include <list>
#include "../resolver/resolverEntityData.h"
#include "../../node_/node.h"
#include "../../node_/nodeFunctionDeclaration.h"
#include <memory>

enum entityType
{
  E_NONE,
  E_FUNCTION,
  E_VARIABLE,
  E_FUNCTION_CALL,
  E_POINTER,
  E_UNARY_ADDRESS,
  E_INDIRECTION,
  E_ARRAY
};

enum codeGenInstruction
{
//If the root entity is a variable, we might have a pointer access to that variable, in the following entities
  CG_POINTER_ACCESS = 0x1,
  CG_FUNCTION_CALL = 0x2
};

class resolverEntity
{
public:
  resolverEntity();
  resolverEntity(std::shared_ptr<node> node);
  void createResolverEntityData();
  void addAddress(std::shared_ptr<node> node);
  void registerFunction(std::shared_ptr<nodeFunctionDeclaration> node);
  std::string getAddress() const;
  std::shared_ptr<resolverEntityData> getResolverEntityData() const { return m_entity_data; }
  std::shared_ptr<node> getNode() const { return m_node; }
  void setEntityType(entityType entity_type) { m_type = entity_type; }
  entityType getEntityType() const { return m_type; }
  std::shared_ptr<datatype> getDatatype() const { return m_datatype; }
  void setDatatype(std::shared_ptr<datatype> datatype) { m_datatype = datatype; }
  void addFunctionArgument(std::shared_ptr<node> node) { m_function_arguments.push_back(node); }
  const std::list<std::shared_ptr<node>>& getFunctionArguments() const { return m_function_arguments; }
  void setFunctionCallStackSize(int stack_size) { m_function_call_stack_size = stack_size; }
  int getFunctionCallStackSize() const { return m_function_call_stack_size; }
  int getCodeGenInstruction() const { return m_code_gen_instruction; }
  void setCodeGenInstruction(codeGenInstruction code_gen_instruction) { m_code_gen_instruction |= code_gen_instruction; }
  void setUnaryIndirectionDepth(int indirection_depth) { m_indirection_depth = indirection_depth; }
  int getUnaryIndirectionDepth() const { return m_indirection_depth; }

private:
  int m_indirection_depth;
  entityType m_type;
  int m_code_gen_instruction;
  std::shared_ptr<resolverEntityData> m_entity_data;
  std::shared_ptr<node> m_node;
  std::shared_ptr<datatype> m_datatype;
  int m_function_call_stack_size;
  std::list<std::shared_ptr<node>> m_function_arguments;
};
