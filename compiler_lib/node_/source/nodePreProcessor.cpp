#include "../nodePreProcessor.h"
#include "../../preprocessor/preprocessor.h"
#include "../../source/pch.h"
#include "../../preprocessor/preprocessorDefinition.h"
#include "../../preprocessor/preProcessorExpresssionable.h"
#include "../../preprocessor/preprocessorExpressionableParser.h"



nodePreProcessor::nodePreProcessor( nodeType node_type, filePosition file_position)
  :node(node_type, file_position),
  m_number_val(0),
  m_preprocessor(0)
{
}



int nodePreProcessor::evaluate()
{
  int result = 0;
  if (getNodeType() == NODE_TYPE_PREPROCESSOR_NUMBER)
  {
    result = evaluateNumber();
  }
  else if (getNodeType() == NODE_TYPE_PREPROCESSOR_IDENTIFIER)
  {
    result = evaluateIdentifier();
  }
  else if (getNodeType() == NODE_TYPE_PREPROCESSOR_UNARY)
  {

  }
  else if (getNodeType() == NODE_TYPE_PREPROCESSOR_PARENTHESES)
  {

  }
  else if (getNodeType() == NODE_TYPE_PREPROCESSOR_EXPRESSION)
  {
    result = evaluateExpression();
  }
  else if (getNodeType() == NODE_TYPE_PREPROCESSOR_EXPRESSION)
  {
    result = evaluateExpression();
  }
  else if (getNodeType() == NODE_TYPE_PREPROCESSOR_JOINED)
  {

  }
  return result;
}

int nodePreProcessor::evaluateIdentifier()
{
  std::shared_ptr<preProcessorDefinition> defintion = m_preprocessor->m_definitions.getDefintion(getStringValue());
  if (!defintion)
  {
    return true;
  }
  if (defintion->getValueTokens()->size() >= 1)
  {
    std::list < std::shared_ptr<token>> tokens_copy = *defintion->getValueTokens();
    if (tokens_copy.size() == 1)
    {
      if (tokens_copy.front()->getNumberValue())
      {
        return tokens_copy.front()->getNumberValue();
      }
    }
    std::shared_ptr< preProcessorExpressionable> collection = std::make_shared< preProcessorExpressionable>();
    collection->setTokens(std::make_shared <std::list < std::shared_ptr<token>>>(tokens_copy));
    m_preprocessor->m_preprocessor_expressionable_parser.parseTokens(collection);
    int val = collection->popLastNode()->evaluate();
    return val;
  }
  else if (defintion->getValueTokens()->size() == 0)
  {
    return false;
  }
  return 0;
}

int nodePreProcessor::evaluateExpression()
{
  long left_operand = getLeftNode()->evaluate();
  if (getRightNode() && (getRightNode()->getNodeType() == NODE_TYPE_PREPROCESSOR_TENARY))
  {
    //handle tenary
  }
  long right_operand = getRightNode()->evaluate();
  return arithmetic(getStringValue(), left_operand, right_operand);
}

int nodePreProcessor::evaluateUnary()
{
  if (STRINGS_EQUAL(getStringValue().c_str(), "!"))
  {
    return !(getRightNode()->evaluate());
  }
  if (STRINGS_EQUAL(getStringValue().c_str(), "~"))
  {
    return ~getRightNode()->evaluate();
  }
  if (STRINGS_EQUAL(getStringValue().c_str(), "-"))
  {
    return -getRightNode()->evaluate();
  }
  cerror("evaluateUnary failed !");
  return 0;
}

int nodePreProcessor::evaluateNumber()
{
  return getNumberValue();
}

void nodePreProcessor::setPreProcessor(preProcessor* preprocessor)
{
  m_preprocessor = preprocessor;
  if (m_left_node)
    m_left_node->setPreProcessor(preprocessor);
  if (m_right_node)
    m_right_node->setPreProcessor(preprocessor);
}

int nodePreProcessor::arithmetic(std::string operator_, long left_operand, long right_operand)
{
  int result = 0;
  if (STRINGS_EQUAL(operator_.c_str(), "*"))
  {
    result = left_operand * right_operand;
  }
  else if (STRINGS_EQUAL(operator_.c_str(), "/"))
  {
    result = left_operand / right_operand;
  }
  else if (STRINGS_EQUAL(operator_.c_str(), "+"))
  {
    result = left_operand + right_operand;
  }
  else if (STRINGS_EQUAL(operator_.c_str(), "-"))
  {
    result = left_operand - right_operand;
  }
  else if (STRINGS_EQUAL(operator_.c_str(), "=="))
  {
    result = left_operand == right_operand;
  }
  else if (STRINGS_EQUAL(operator_.c_str(), "!="))
  {
    result = left_operand != right_operand;
  }
  else if (STRINGS_EQUAL(operator_.c_str(), ">"))
  {
    result = left_operand > right_operand;
  }
  else if (STRINGS_EQUAL(operator_.c_str(), "<"))
  {
    result = left_operand < right_operand;
  }
  else if (STRINGS_EQUAL(operator_.c_str(), ">="))
  {
    result = left_operand >= right_operand;
  }
  else if (STRINGS_EQUAL(operator_.c_str(), "<="))
  {
    result = left_operand <= right_operand;
  }
  else if (STRINGS_EQUAL(operator_.c_str(), "<<"))
  {
    result = left_operand << right_operand;
  }
  else if (STRINGS_EQUAL(operator_.c_str(), ">>"))
  {
    result = left_operand >> right_operand;
  }
  else if (STRINGS_EQUAL(operator_.c_str(), "&&"))
  {
    result = left_operand && right_operand;
  }
  else if (STRINGS_EQUAL(operator_.c_str(), "||"))
  {
    result = left_operand || right_operand;
  }
  else
  {
    cerror("failed to process pre processor arithmetic!");
  }
  return result;
}
