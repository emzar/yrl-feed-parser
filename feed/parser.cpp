#include "parser.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <glibmm/convert.h>
#include <iostream>
#include <unordered_set>

namespace realty {
namespace feed {

static const size_t PARSE_FEED_BUFFER_SIZE = 4096;

int i = 0;

void test_offer_callback(offer_node_ptr&& offer)
{
  std::cout << "test_offer_callback: " << offer->data() << " i: " << i++ << std::endl;
}

void parse_feed(const std::string& fname)
{
  std::ifstream ifs(fname);
  char buffer[PARSE_FEED_BUFFER_SIZE];
  realty::feed::parser parser(std::move(test_offer_callback));
  parser.set_substitute_entities(true);
  do {
    std::memset(buffer, 0, PARSE_FEED_BUFFER_SIZE);
    ifs.read(buffer, PARSE_FEED_BUFFER_SIZE - 1);
    if (ifs.gcount()) {
      // We use Glib::ustring::ustring(InputIterator begin, InputIterator end)
      // instead of Glib::ustring::ustring( const char*, size_type ) because it
      // expects the length of the string in characters, not in bytes.
      Glib::ustring input(buffer, buffer + ifs.gcount());
      parser.parse_chunk(input);
    }
  }
  while (ifs);
}

parser::parser(fn_offer_callback&& offer_callback)
  : xmlpp::SaxParser(), m_offer_callback(offer_callback)
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
    m_offer_root->data(attr->value);
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
    m_offer_callback(std::move(m_offer_root));
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
