#include "parser.h"

#include <algorithm>

namespace realty {
namespace feed {

parser::parser(fn_offer_callback&& offer_callback, const std::string& feed_id)
  : xmlpp::SaxParser(), m_offer_callback(offer_callback), m_feed_id(feed_id)
{
  std::locale::global(std::locale(""));
}

void parser::on_start_element(
  const Glib::ustring& name, const AttributeList& attributes)
{
  if (name == "offer") {
    m_offer_root = std::make_shared<offer_node>();
    m_offer_root->name(name);
    AttributeList::const_iterator attr = std::find_if(
      attributes.begin(), attributes.end(),
      [](AttributeList::const_reference attr)
      { return (attr.name == "internal-id");});
    if (attr != attributes.end()) {
      offer_node_ptr internal_id = std::make_shared<offer_node>();
      internal_id->name(attr->name);
      internal_id->data(attr->value);
      m_offer_root->add_child(internal_id);
    }

    offer_node_ptr feed_id = std::make_shared<offer_node>();
    feed_id->name("feed_id");
    feed_id->data(Glib::ustring(m_feed_id));
    m_offer_root->add_child(feed_id);

    m_current_offer_node = m_offer_root;
  }
  else if (m_current_offer_node != nullptr) {
    offer_node_ptr new_node = std::make_shared<offer_node>();
    new_node->name(name);
    m_current_offer_node->add_child(new_node);
    m_current_offer_node = new_node;
  }
}

void parser::on_end_element(const Glib::ustring& name)
{
  if (name == "offer") {
    m_offer_callback(std::move(*m_offer_root));
    m_current_offer_node.reset();
  }
  else if (m_current_offer_node != nullptr) {
    m_current_offer_node = m_current_offer_node->parent();
  }
}

void parser::on_characters(const Glib::ustring& text)
{
  if ((m_current_offer_node != nullptr)
    && (m_current_offer_node->name() != "offer")) {
    m_current_offer_node->data(text);
  }
}

} // namespace feed
} // namespace realty
