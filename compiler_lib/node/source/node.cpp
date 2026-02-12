#include "../../source/pch.h"
#include "../node.h"
#include "../../braze_compiler.h"

node::node()
     : m_node_type(nodeType::NODE_TYPE_BLANK)
{
}

node::node(nodeType node_type, filePosition file_position)
    : m_node_type(node_type),
      m_file_position(file_position)
{
}


