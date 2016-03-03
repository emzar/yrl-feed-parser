#ifndef REALTY_FEED_OFFER_NODE_H
#define REALTY_FEED_OFFER_NODE_H

#include <libxml++/libxml++.h>
#include <unordered_map>

namespace realty {
namespace feed {

class offer_node;

typedef std::shared_ptr<offer_node> offer_node_ptr;
typedef std::weak_ptr<offer_node> offer_node_weak_ptr;
typedef std::unordered_map<std::string, offer_node_ptr> offer_children_type;

class offer_node : public std::enable_shared_from_this<offer_node>
{
public:
  offer_node() : std::enable_shared_from_this<offer_node>() {}
  virtual ~offer_node() {};
  void add_child(offer_node_ptr node);
  offer_node_ptr parent() noexcept { return m_parent.lock(); }
  const Glib::ustring& name() const noexcept { return m_name; };
  const Glib::ustring& data() const noexcept { return m_data; };
  void name(const Glib::ustring& name) noexcept { m_name = name; }
  void data(const Glib::ustring& data) noexcept { m_data = data; }
  bool has_children() const noexcept { return !m_children.empty(); }
  offer_children_type& children() noexcept { return m_children; };
  offer_node& operator[] (const std::string& child_name) { return *m_children[child_name]; }
  offer_node& operator[] (std::string&& child_name) { return *m_children[child_name]; }
  operator const Glib::ustring& () const { return m_data; }

private:
  offer_node_weak_ptr m_parent;
  offer_children_type m_children;
  Glib::ustring m_name;
  Glib::ustring m_data;
};

} // namespace feed
} // namespace realty

#endif // REALTY_FEED_OFFER_NODE_H
