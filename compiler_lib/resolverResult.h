#pragma once
#include "resolverEntity.h"
class resolverResult
{
public:
  
  void addEntity(std::shared_ptr < resolverEntity > entity);
  std::shared_ptr < resolverEntity > peekEntity();
  std::shared_ptr < resolverEntity > nextEntity();

  std::shared_ptr < resolverEntity > m_root_entity;
  std::shared_ptr < resolverEntity > m_entity;
  std::shared_ptr < resolverEntity > m_last_entity;


};

