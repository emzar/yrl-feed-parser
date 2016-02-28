#include "offer_tree.h"

#include <iostream>

namespace realty {

void offer_node::add_child(offer_node_ptr node)
{
  node->m_parent = shared_from_this();
  m_children.push_back(node);
}

} // namespace realty
