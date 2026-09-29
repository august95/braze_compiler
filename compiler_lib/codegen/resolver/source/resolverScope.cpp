#include "../../../source/pch.h"
#include "../../../braze_compiler.h"
#include "../resolverScope.h"

resolverScope::resolverScope(std::shared_ptr<resolverScope> parent)
    : m_parent(std::move(parent))
{
}

void resolverScope::addScopeEntity(std::shared_ptr<resolverEntity> scope_data)
{
  m_scope_entities.push_back(scope_data);
}

void resolverScope::follow(std::shared_ptr<node> node, std::shared_ptr<resolverResult> result)
{
  if (!node)
  {
    return;
  }

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
  if (!node)
  {
    return;
  }

  for (const auto& entity : m_scope_entities)
  {
    const auto& declaration = entity->getNode();
    if (declaration && declaration->getStringValue() == node->getStringValue())
    {
      result->addEntity(entity);
      return;
    }
  }

  if (m_parent)
  {
    m_parent->followName(node, result);
    return;
  }

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
  std::shared_ptr<resolverEntity> function_entity = result->peekLastEntity();
  if (!function_entity)
  {
    cerror("could not resolve function!");
    return;
  }
  std::shared_ptr<resolverEntity> function_call_entity = std::make_shared<resolverEntity>();
  function_call_entity->setEntityType(E_FUNCTION_CALL);
  function_entity->setCodeGenInstruction(CG_FUNCTION_CALL);
  result->addEntity(function_call_entity);

  if (node_->getRightNode())
  {
    function_call_entity->setFunctionCallStackSize(
        buildFunctionCallArguments(node_->getRightNode(), function_call_entity));
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
  last_entity->setCodeGenInstruction(CG_POINTER_ACCESS);
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

int resolverScope::buildFunctionCallArguments(
    std::shared_ptr<nodeExpression> node,
    std::shared_ptr<resolverEntity> function_call_entity)
{
  if (!node)
  {
    return 0;
  }
  if (node->getStringValue() == ",")
  {
    return buildFunctionCallArguments(node->getLeftNode(), function_call_entity) +
        buildFunctionCallArguments(node->getRightNode(), function_call_entity);
  }
  if (node->getNodeType() == NODE_TYPE_EXPRESSION_PARANTHESES)
  {
    return buildFunctionCallArguments(node->getParenthesesNode(), function_call_entity);
  }

  function_call_entity->addFunctionArgument(node);
  if (!node->getDatatype())
  {
    cerror("function argument dont have a datatype size!");
    return 0;
  }
  const int datatype_size = node->getDatatype()->getDatatypeSize();
  return datatype_size + datatype::Padding(datatype_size, DATA_SIZE_DWORD);
}
