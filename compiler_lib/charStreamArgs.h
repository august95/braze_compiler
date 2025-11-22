#pragma once
#include "charStreamInterface.h"
#include <sstream>
#include <memory>

class charStreamArgs :
    public charStreamInterface
{
  public:
  charStreamArgs() {};
  virtual ~charStreamArgs(){};
  void initialize(std::shared_ptr<std::stringstream> input_args);
  virtual char peekChar() override;
  virtual char nextChar() override;
  virtual void pushChar(char ch) override;
  virtual bool initialized() override;
protected:
  std::shared_ptr<std::stringstream> m_input_args;
};

