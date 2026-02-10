#pragma once
#include "charStreamInterface.h"
#include <iostream>
#include <fstream>

class charStreamFile :
  public charStreamInterface
{
public:
  charStreamFile() {};
  virtual ~charStreamFile();
  void initialize(std::string filename);
  virtual char peekChar() override;
  virtual char nextChar() override;
  virtual void pushChar(char ch) override;
  virtual bool initialized() override;
  virtual void close() override;
private:
  std::ifstream m_file;
  std::string m_filename;
};

