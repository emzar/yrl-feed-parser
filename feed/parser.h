#ifndef REALTY_FEED_PARSER_H
#define REALTY_FEED_PARSER_H

#include <libxml++/libxml++.h>

#include "offer_node.h"

namespace realty {
namespace feed {

typedef std::function<void (offer_node_ptr&& offer)> fn_offer_callback;

class parser : public xmlpp::SaxParser
{
public:
  parser(fn_offer_callback&& offer_callback);
  ~parser() override {}

protected:
  void on_start_document() override {}
  void on_end_document() override {}
  void on_start_element(
    const Glib::ustring& name, const AttributeList& properties) override;
  void on_end_element(const Glib::ustring& name) override;
  void on_characters(const Glib::ustring& characters) override;
  void on_comment(const Glib::ustring& text) override {}
  void on_warning(const Glib::ustring& text) override {}
  void on_error(const Glib::ustring& text) override {}
  void on_fatal_error(const Glib::ustring& text) override {}

private:
  offer_node_ptr m_offer_root;
  offer_node_ptr m_current_offer_node;
  fn_offer_callback m_offer_callback;
};

} // namespace feed
} // namespace realty

#endif // REALTY_FEED_PARSER_H
