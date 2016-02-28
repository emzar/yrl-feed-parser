#ifndef REALTY_OFFER_TREE_H
#define REALTY_OFFER_TREE_H

#include <libxml++/libxml++.h>
#include <list>

namespace realty {

class offer_node;

typedef std::shared_ptr<offer_node> offer_node_ptr;
typedef std::weak_ptr<offer_node> offer_node_weak_ptr;
typedef std::list<offer_node_ptr> offer_children_type;

class offer_node : public std::enable_shared_from_this<offer_node>
{
public:
  offer_node() : std::enable_shared_from_this<offer_node>() {}
  virtual ~offer_node() {};

  void add_child(offer_node_ptr node);
  offer_node_ptr parent() { return m_parent.lock(); }
  offer_children_type& children() { return m_children; };
  const Glib::ustring& name() const { return m_name; };
  const Glib::ustring& data() const { return m_data; };
  void name(const Glib::ustring& name) { m_name = name; }
  void data(const Glib::ustring& data) { m_data = data; }

private:
  offer_node_weak_ptr m_parent;
  offer_children_type m_children;
  Glib::ustring m_name;
  Glib::ustring m_data;
};

} // namespace realty

#endif // REALTY_OFFER_TREE_H
