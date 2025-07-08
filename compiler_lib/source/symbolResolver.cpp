#include "pch.h"
#include <assert.h>
#include "../symbolResolver.h"
#include "../braze_compiler.h"

symbolResolver::symbolResolver()
	:m_root_scope(std::make_shared< scope >())
{
	m_root_scope->init(std::shared_ptr<scope>(), true);
	m_last_scope = m_root_scope;
}

void symbolResolver::finishScope()
{
	if (m_last_scope->isRootScope())
	{
		cerror("tried to delete root scope!");
		assert(0);
	}
	m_last_scope = m_last_scope->getParent();
}

void symbolResolver::addNodeToCurrentScope(std::shared_ptr<node> node)
{
	m_last_scope->addNode(node);
}

void symbolResolver::newScope()
{
	std::shared_ptr<scope> _scope = std::make_shared<scope>();
	_scope->init(m_last_scope);
	m_last_scope = _scope;
}

std::shared_ptr<node> symbolResolver::findDeclerationNode(std::shared_ptr<node> identifier)
{
	return m_last_scope->find(identifier);
}

std::shared_ptr<node> symbolResolver::findVariableDeclerationNode(std::shared_ptr<node> identifier)
{
	return std::shared_ptr<node>();
}

std::shared_ptr<node> symbolResolver::findFunctionNode(std::shared_ptr<node> identifier)
{
	return std::shared_ptr<node>();
}
