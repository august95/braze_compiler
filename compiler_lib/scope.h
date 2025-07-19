#pragma once
#include <iostream>
#include <list>
#include "node.h"
#include <memory>

// maybe calculate all scope offsets after parsing in the future?
class scope
{
public:
	scope();

	void init(std::shared_ptr<scope> last_scope, bool is_root_scope = false);
	void addNode(std::shared_ptr<node> node);
	std::shared_ptr<scope> getParent() { return m_parent; }
	bool isRootScope() { return m_is_root_scope; }
	std::shared_ptr<node> find(std::shared_ptr<node> identifier);

private:
	int m_flags;
	size_t m_size;
	bool m_is_root_scope;
	std::list<std::shared_ptr<node>> m_scope_nodes;
	std::shared_ptr<scope> m_parent;
};
