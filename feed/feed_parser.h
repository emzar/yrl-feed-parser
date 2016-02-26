#ifndef REALTY_PARSER_H
#define REALTY_PARSER_H

#include <libxml++/libxml++.h>

namespace realty {

void parse_feed(const std::string& fname);

class feed_parser : public xmlpp::SaxParser
{
public:
  feed_parser();
  ~feed_parser() override;

protected:
  //overrides:
  void on_start_document() override;
  void on_end_document() override;
  void on_start_element(
    const Glib::ustring& name, const AttributeList& properties) override;
  void on_end_element(const Glib::ustring& name) override;
  void on_characters(const Glib::ustring& characters) override;
  void on_comment(const Glib::ustring& text) override;
  void on_warning(const Glib::ustring& text) override;
  void on_error(const Glib::ustring& text) override;
  void on_fatal_error(const Glib::ustring& text) override;
};

} // namespace realty

#endif // REALTY_PARSER_H
