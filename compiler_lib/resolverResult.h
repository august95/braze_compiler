#pragma once
#include "resolverEntity.h"
#include <list>
#include <string>
class resolverResult
{
public:
  
  void addEntity(std::shared_ptr < resolverEntity > entity);
  std::shared_ptr < resolverEntity > peekEntity();
  std::shared_ptr < resolverEntity > nextEntity();
  std::shared_ptr < resolverEntity > getRootEntity();
  std::string getRootAddress();

private:
  std::list < std::shared_ptr < resolverEntity >> m_entities;
  std::shared_ptr < resolverEntityData > m_root_entity_data;


};

