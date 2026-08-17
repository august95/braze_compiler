#include "../../source/pch.h"
#include "../preprocessor.h"
#include "../preProcessorExpresssionable.h"
#include <list>
preProcessor::preProcessor()
  :m_preprocessor_expressionable_parser(this),
   m_enabled(1)
{

}

int preProcessor::startPreProcessor()
{
  if (!m_enabled)
  {
    m_tokens_pre_processed = m_tokens_original;
    return 0;
  }
  m_tokens_pre_processed = std::make_shared  < std::list < std::shared_ptr < token > > >();
  while (!m_tokens_original->empty())
  {
    handleToken();
  }
  return 0;
}

std::shared_ptr<token> preProcessor::nextToken()
{
  if (m_tokens_original->empty())
    return 0;

  std::shared_ptr<token> token = m_tokens_original->front();
  m_tokens_original->pop_front();
  return token;
}

std::shared_ptr<token> preProcessor::peekToken()
{
  if (m_tokens_original->empty())
    return 0;
  return m_tokens_original->front();
}

void preProcessor::pushToken(std::shared_ptr<token> token)
{
  m_tokens_original->push_front(token);
}


void preProcessor::pushPreProcessedToken(std::shared_ptr<token> token)
{
  m_tokens_pre_processed->push_back(token);
}

void preProcessor::handleToken()
{
  std::shared_ptr<token> token = peekToken();
  if (token->isTokenTypeSymbol())
  {
    handleSymbol();
  }
  else if (token->isTokenTypeIdentifier())
  {
    handleIdentifier();
  }
  /*
  else if (token->isTokenTypeKeyword())
  {

  }
  else if (token->isTokenTypeNewLine())
  {

  }
  */
  else
  {
    m_tokens_pre_processed->push_back(nextToken());
  }
}

void preProcessor::handleIdentifier()
{
  std::shared_ptr<token> token = nextToken();
  std::shared_ptr<preProcessorDefinition> definition = m_definitions.getDefintion(token->getStringValue());
  if (!!definition)
  {
    for (auto it = definition->getValueTokens()->begin(); it != definition->getValueTokens()->end(); it++)
      m_tokens_pre_processed->push_back(*it);
  }
  if (!definition)
  {
    m_tokens_pre_processed->push_back(token);
  }
}

void preProcessor::handleSymbol()
{
  std::shared_ptr<token> token = nextToken();
  bool is_processed = false;
  if (token->getCharValue() == '#')
  {
    is_processed = handleHashtagToken();
  }
  if (!is_processed)
  {
    m_tokens_pre_processed->push_back(token);
  }
}

bool preProcessor::handleHashtagToken()
{
  bool is_processed = false;
  std::shared_ptr<token> token_ = nextToken();
  assert(token_->isTokenTypeKeyword());

  if (STRINGS_EQUAL(token_->getStringValue().c_str(), "define"))
  {
    handleDefinitionToken();
    is_processed =  true;
  }
  else if (STRINGS_EQUAL(token_->getStringValue().c_str(), "if"))
  {
    handleIfToken();
    is_processed =  true;
  }
  if (STRINGS_EQUAL(token_->getStringValue().c_str(), "ifdef"))
  {
    handleIfDef();
    is_processed =  true;
  }
  else if (STRINGS_EQUAL(token_->getStringValue().c_str(), "ifndef"))
  {
    handleIfNDef();
    is_processed =  true;
  }
  else if (STRINGS_EQUAL(token_->getStringValue().c_str(), "endif"))
  {
    //skipToEndif or readToEndif has been called previously in the call stack
    //push back the endif, and let these functions deal with it!
    pushToken(token_);
    is_processed = true;
  }
  else if (STRINGS_EQUAL(token_->getStringValue().c_str(), "undef"))
  {
    undef();
    is_processed =  true;
  }
  else if (STRINGS_EQUAL(token_->getStringValue().c_str(), "warning"))
  {
    handleWarning();
    is_processed =  true;
  }
  else if (STRINGS_EQUAL(token_->getStringValue().c_str(), "error"))
  {
    handleError();
    is_processed =  true;
  }

  return is_processed;
}

void preProcessor::handleIfToken()
{
  int result = parseAndEvaluate();
  readToEndif(result);
}

void preProcessor::handleDefinitionToken()
{
  std::shared_ptr<token> name_token = nextToken();
  std::shared_ptr < std::list < std::shared_ptr<token> > > argument_tokens;
  //fixme: process macro arguments

  std::shared_ptr < std::list<std::shared_ptr<token>>> definition_value = std::make_shared < std::list<std::shared_ptr<token>>>( handleDefinitionValue());

  m_definitions.addDefinition(name_token->getStringValue(), definition_value, argument_tokens);

}

void preProcessor::handleIfDef()
{
  std::shared_ptr<token> token = nextToken();
  std::shared_ptr<preProcessorDefinition> definition = m_definitions.getDefintion(token->getStringValue());
  readToEndif(!!definition);
}

void preProcessor::handleIfNDef()
{
  std::shared_ptr<token> token = nextToken();
  std::shared_ptr<preProcessorDefinition> definition = m_definitions.getDefintion(token->getStringValue());
  readToEndif(!definition);
}

void preProcessor::undef()
{
  std::shared_ptr<token> token = nextToken();
  m_definitions.removeDefinition(token->getStringValue());
}

void preProcessor::handleWarning()
{
  std::shared_ptr<token> token = nextToken();
  cwarning(token->getStringValue().c_str());
}

void preProcessor::handleError()
{
  std::shared_ptr<token> token = nextToken();
  cerror(token->getStringValue().c_str());
  exit(-1);
}

std::list<std::shared_ptr<token>> preProcessor::handleDefinitionValue()
{
  std::list<std::shared_ptr<token>> multi_value_definition;
  std::shared_ptr<token> token = nextToken();
  while (token)
  {
    if (token->isTokenTypeNewLine())
    {
      break;
    }
    if (token->isTokenTypeSymbol() && token->getCharValue() == '\\')
    {
      nextToken(); //skip new line \n
      token = nextToken();
      continue;
    }
    multi_value_definition.push_back(token);
    token = nextToken();
  }
  return multi_value_definition;
}

//removes nested if/ifndef/ifdef that are already going to be removed by a parent if/ifndef/ifdef+
void preProcessor::skipToEndif()
{
  std::shared_ptr<token> token = peekToken();
  while (token && !(token->isTokenTypeKeyword() && STRINGS_EQUAL(token->getStringValue().c_str(), "endif")))
  {
    if (token->isTokenTypeKeyword() && (
      STRINGS_EQUAL(token->getStringValue().c_str(), "if") ||
      STRINGS_EQUAL(token->getStringValue().c_str(), "ifdef") ||
      STRINGS_EQUAL(token->getStringValue().c_str(), "ifndef")))
    {
      skipToEndif();
    }
    token = nextToken();
  }
}

void preProcessor::readToEndif(bool condition)
{
  std::shared_ptr<token> token = peekToken();
  while (token && !(token->isTokenTypeKeyword() && STRINGS_EQUAL(token->getStringValue().c_str(), "endif")))
  {
    if (condition)
    {
      handleToken();
      token = peekToken();
      continue;
    }

    if (token->isTokenTypeKeyword() && (
      STRINGS_EQUAL(token->getStringValue().c_str(), "if") ||
      STRINGS_EQUAL(token->getStringValue().c_str(), "ifdef") ||
      STRINGS_EQUAL(token->getStringValue().c_str(), "ifndef")))
    {
      skipToEndif();
    }
    token = nextToken();
  }
  token = nextToken(); // pop endif;
}

int preProcessor::parseAndEvaluate(){
  std::shared_ptr< preProcessorExpressionable> collection = std::make_shared< preProcessorExpressionable>();
  collection->setTokens(m_tokens_original);
  m_preprocessor_expressionable_parser.parseTokens(collection);
  std::shared_ptr <nodePreProcessor> node_ = collection->popLastNode();
  int val = node_->evaluate();
  return val;
}

void preProcessor::definitions::addDefinition(std::string name, std::shared_ptr < std::list<std::shared_ptr<token>>> value_tokens, std::shared_ptr < std::list<std::shared_ptr<token>>> argument_tokens)
{
  removeDefinition(name);//remove old defintion
  std::shared_ptr<preProcessorDefinition> pre_processor_definition = std::make_shared< preProcessorDefinition>(preProcessorDefinition::type::PREPROCESSOR_DEFINITION_STANDARD);
  pre_processor_definition->setName(name);
  pre_processor_definition->setValueTokens(value_tokens);
  pre_processor_definition->setArgumentTokens(argument_tokens);
  if (argument_tokens)
  {
    pre_processor_definition->setType(preProcessorDefinition::type::PREPROCESSOR_DEFINITION_MACRO_FUNCTION);
  }
  m_defintions.push_back(pre_processor_definition);
}

void preProcessor::definitions::removeDefinition(std::string name)
{
  for (auto it = m_defintions.begin(); it != m_defintions.end();)
  {
    if (*it && (*it)->getName() == name)
    {
      it = m_defintions.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

std::shared_ptr<preProcessorDefinition> preProcessor::definitions::getDefintion(std::string name)
{
  for (auto it = m_defintions.begin(); it != m_defintions.end();    it++)
  {
    if (*it && (*it)->getName() == name)
    {
      return *it;
    }
  }
}
