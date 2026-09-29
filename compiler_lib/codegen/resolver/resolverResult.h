#pragma once
#include "resolverEntity.h"
#include <list>
#include <string>
#include <memory>

class resolverResult
{
public:
  void addEntity(std::shared_ptr<resolverEntity> entity);
  std::shared_ptr<resolverEntity> peekLastEntity() const;
  std::shared_ptr<resolverEntity> popLastEntity();
  std::shared_ptr<resolverEntity> getRootEntity();
  std::string getRootAddress() const;

private:
  std::list<std::shared_ptr<resolverEntity>> m_entities;
};
