#include "../charStreamArgs.h"

void charStreamArgs::initialize(std::shared_ptr<std::stringstream> input_args)
{
  m_input_args=input_args;
}

char charStreamArgs::peekChar()
{
  return m_input_args->peek();
}

char charStreamArgs::nextChar()
{
  return m_input_args->get();
}

void charStreamArgs::pushChar(char ch)
{
  m_input_args->putback(ch);
}

bool charStreamArgs::initialized()
{
  return m_input_args->peek() != EOF;
}
