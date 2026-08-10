#pragma once

#include "../braze_compiler.h"
#include <string>

class preProcessorDefinition
{
public:
  enum type
  {
    PREPROCESSOR_DEFINITION_STANDARD,
    PREPROCESSOR_DEFINITION_MACRO_FUNCTION,
    PREPROCESSOR_DEFINITION_NATIVE_CALLBACK,
    PREPROCESSOR_DEFINITION_TYPEDEF
  };

  preProcessorDefinition(type type);

  void setValueTokens(std::shared_ptr < std::list<std::shared_ptr<token>>> value_tokens) { m_value_tokens = value_tokens; }
  std::shared_ptr < std::list<std::shared_ptr<token>>> getValueTokens() { return m_value_tokens; }
  void setArgumentTokens(std::shared_ptr<std::list<std::shared_ptr<token>>> argument_tokens) { m_argument_tokens = argument_tokens; }
  std::shared_ptr < std::list<std::shared_ptr<token>>> getArgumentTokens() { return m_argument_tokens; }
  void setName(std::string name) { m_name = name; }
  std::string getName() { return m_name; }
  void setType(type type) { m_type = type; }
  type getType() { return m_type; }

private:

  std::shared_ptr < std::list < std::shared_ptr<token> >> m_value_tokens;
  std::shared_ptr < std::list < std::shared_ptr<token> >> m_argument_tokens;

  std::string m_name;
  type m_type;
};