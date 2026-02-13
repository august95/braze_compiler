#include "../../../source/pch.h"
#include "../../../braze_compiler.h"
#include "../resolverScope.h"

resolverScope::resolverScope()
    : m_root_scope(false),
      m_is_local_stack(false),
      m_is_stack(false),
      flags(false)
{
}

void resolverScope::addScopeEntity(std::shared_ptr<resolverEntity> scope_data)
{
  m_scope_entities.push_back(scope_data);
}

void resolverScope::follow(std::shared_ptr<node> node, std::shared_ptr<resolverResult> result)
{
  if (node->getNodeType() == NODE_TYPE_VARIABLE)
  {
    followName(node, result);
  }
  else if (node->getNodeType() == NODE_TYPE_IDENTIFIER)
  {
    followName(cast_node<nodeExpression>(node), result);
  }
  else if (node->getNodeType() == NODE_TYPE_EXPRESSION)
  {
    followExpression(cast_node<nodeExpression>(node), result);
  }
  else if (node->getNodeType() == NODE_TYPE_EXPRESSION_PARANTHESES)
  {
    followExpression((cast_node<nodeExpression>(node)->getParenthesesNode()), result);
  }
  else if (node->getNodeType() == NODE_TYPE_UNARY)
  {
    followUnary(cast_node<nodeExpression>(node), result);
  }
}

void resolverScope::followName(std::shared_ptr<node> node, std::shared_ptr<resolverResult> result)
{
  for (auto entity : m_scope_entities)
  {
    if (!entity->getNode()->getStringValue().empty())
    {
      std::string entity_s = entity->getNode()->getStringValue().c_str();
      std::string node_s = node->getStringValue().c_str();
      if (STRINGS_EQUAL(entity_s.c_str(), node_s.c_str()))
      {
        result->addEntity(entity);
        return;
      }
    }
  }
  if (m_root_scope != true)
    return m_prev_scope->follow(node, result);
  else
    cerror("searched for node past root scope!");
  assert(0);
}

void resolverScope::followExpression(std::shared_ptr<nodeExpression> node, std::shared_ptr<resolverResult> result)
{

  if (STRINGS_EQUAL(node->getStringValue().c_str(), "()"))
  {
    followFunctionCall(node, result);
  }
}

void resolverScope::followFunctionCall(std::shared_ptr<nodeExpression> node_, std::shared_ptr<resolverResult> result)
{
  assert(node_->getLeftNode()->getNodeType() == NODE_TYPE_IDENTIFIER);
  std::shared_ptr<node> func_name = node_->getLeftNode();

  follow(func_name, result);
  std::shared_ptr<resolverEntity> function_entity = result->peekEntity();
  if (!function_entity)
  {
    cerror("could not resolve function!");
  }
  std::shared_ptr<resolverEntity> function_call_entity = std::make_shared<resolverEntity>();
  function_call_entity->setEntityType(E_FUNCTION_CALL);
  function_entity->setCodeGenInstruction(CG_LOAD_FUNCTION_ADDRESS_TO_EBX);
  result->addEntity(function_call_entity);

  if (node_->getRightNode())
  {
    int function_call_stack_size = 0;
    buildFunctionCallArguments(node_->getRightNode(), function_call_entity, result, function_call_stack_size);
    function_call_entity->setFunctionCallStacksize(function_call_stack_size);
  }
  function_call_entity->setDatatype(function_entity->getDatatype());
}

void resolverScope::followUnary(std::shared_ptr<nodeExpression> node_, std::shared_ptr<resolverResult> result)
{
  //indirection
  if (STRINGS_EQUAL(node_->getStringValue().c_str(), "*"))
  {
    followUnaryIndirection(node_, result);
  }
  //address
  else if (STRINGS_EQUAL(node_->getStringValue().c_str(), "&"))
  {
    followUnaryAddress(node_, result);
  }

}

void resolverScope::followUnaryAddress(std::shared_ptr<nodeExpression> node_, std::shared_ptr<resolverResult> result)
{
  // int val;
  // int ptr* = &val;
  // we are creating a pointer out of the identifier val
  follow(node_->getValueNode(), result);
  std::shared_ptr<resolverEntity> last_entity = result->getRootEntity();
  std::shared_ptr<resolverEntity> unary_address = std::make_shared<resolverEntity>(node_);
  unary_address->setEntityType(E_UNARY_ADDRESS);
  last_entity->setDatatype(last_entity->getNode()->getDatatype());
  unary_address->setDatatype(last_entity->getNode()->getDatatype());
  //load the address of the variable val into register
  last_entity->setCodeGenInstruction(CG_LOAD_VALUE_TO_EBX);
  result->addEntity(unary_address);
}

void resolverScope::followUnaryIndirection(std::shared_ptr<nodeExpression> node_, std::shared_ptr<resolverResult> result)
{
  follow(node_->getValueNode(), result);
  std::shared_ptr<resolverEntity> indirection_entity = std::make_shared<resolverEntity>(node_);
  std::shared_ptr<resolverEntity> last_entity = result->getRootEntity();
  indirection_entity->setEntityType(E_INDIRECTION);
  indirection_entity->setUnaryIndirectionDepth(node_->getUnaryIndirectionDepth());
  indirection_entity->setDatatype(node_->getDatatype());
  last_entity->setDatatype(node_->getDatatype());
  result->addEntity(indirection_entity);
}

void resolverScope::buildFunctionCallArguments(std::shared_ptr<nodeExpression> node_, std::shared_ptr<resolverEntity> function_call_entity, std::shared_ptr<resolverResult> result, int &function_call_stack_size)
{
  // we have multiple arguments separated by opertaor node wiht op ",
  if (!node_)
  {
    return;
  }
  if (STRINGS_EQUAL(node_->getStringValue().c_str(), ","))
  {
    buildFunctionCallArguments(node_->getLeftNode(), function_call_entity, result, function_call_stack_size);
    buildFunctionCallArguments(node_->getRightNode(), function_call_entity, result, function_call_stack_size);
  }
  else if (node_->getNodeType() == NODE_TYPE_EXPRESSION_PARANTHESES)
  {
    buildFunctionCallArguments(node_->getParenthesesNode(), function_call_entity, result, function_call_stack_size);
  }
  else
  {
    function_call_entity->addFunctionArgumnet(node_);
    int datatype_size = 0;
    if (node_->getDatatype())
    {
      datatype_size = node_->getDatatype()->getDatatypeSize();
    }
    else
    {
      cerror("function argument dont have a dataype size!!");
    }
    function_call_stack_size += (datatype_size + datatype::Padding(datatype_size, DATA_SIZE_DWORD));
  }
}
