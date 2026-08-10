#pragma once

#include <string>
#include <list>
#include "../braze_compiler.h"
#include "../precedenceHandler.h"
#include "preProcessorExpresssionable.h"
#include "../node_/nodePreProcessor.h"



class preProcessorExpressionableParser
{
public:
  preProcessorExpressionableParser(class preProcessor* preprocessor);


  void parseUnary(std::shared_ptr< preProcessorExpressionable> expressionable);
  void parseExpression(std::shared_ptr< preProcessorExpressionable> expressionable);
  bool parseOperand(std::shared_ptr< preProcessorExpressionable> expressionable);
  void parseOperator(std::shared_ptr< preProcessorExpressionable> expressionable);
  bool parseExpressionOperatorOrOperand(std::shared_ptr< preProcessorExpressionable> expressionable);
  bool parseNextToken(std::shared_ptr< preProcessorExpressionable> expressionable);
  void parseTokens(std::shared_ptr< preProcessorExpressionable> expressionable);
  

  class preProcessor* m_preprocessor;
  
};