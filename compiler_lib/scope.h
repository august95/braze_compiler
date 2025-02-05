#pragma once
#include <iostream>
#include <list>
#include "scopeEntity.h"

//maybe calculate all scope offsets after parsing in the future?
class scope
{
public:
	scope();

	void init(std::shared_ptr < scope > last_scope, bool is_root_scope = false);
	void addNode(std::shared_ptr<node> node);
	void updateStackOffset(std::shared_ptr<node> node, int stack_offset);
	std::shared_ptr < scope > getParent() { return m_parent; }
	bool isRootScope() { return m_is_root_scope; }
private:
	int m_flags;
	size_t m_size;
	bool m_is_root_scope;
	std::list < std::shared_ptr < scopeEntity > > m_scope_entities;
	std::shared_ptr < scope > m_parent;
};
