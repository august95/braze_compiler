#pragma once
class resolverScope
{
public:
  resolverScope();
  void setNextScope(std::shared_ptr <resolverScope> next_scope) { m_next_scope = next_scope; }
  std::shared_ptr < resolverScope> getNextScope() { return m_next_scope; }
private:
  std::shared_ptr <resolverScope> m_next_scope;
  std::shared_ptr <resolverScope> m_prev_scope;
};

