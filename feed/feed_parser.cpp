#include "feed_parser.h"

#include <cstring>
#include <fstream>
#include <glibmm/convert.h>
#include <iostream>
#include <unordered_set>

namespace realty {

std::unordered_set<std::string> required_elements =
  {"yandex-­building-­id",
   "price",
   "value",
   "currency",
   "area",
   "unit",
   "living­-space",
   "kitchen­-area",
   "renovation",
   "image",
   "rooms",
   "bathroom­-unit",
   "floor",
   "floors­-total",
   "building-­type",
   "building­-state",
   "built­-year",
   "ready­-quarter",
   "building-­phase",
   "building­-section",
   "ceiling-­height"};

static const size_t PARSE_FEED_BUFFER_SIZE = 4096;

void parse_feed(const std::string& fname)
{
  std::ifstream ifs(fname);
  char buffer[PARSE_FEED_BUFFER_SIZE];
  realty::feed_parser parser;
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

feed_parser::feed_parser()
  : xmlpp::SaxParser()
{
  // Empty
}

feed_parser::~feed_parser()
{
  // Empty
}

void feed_parser::on_start_document()
{
  std::cout << "on_start_document" << std::endl;
}

void feed_parser::on_end_document()
{
  std::cout << "on_end_document" << std::endl;
}

void feed_parser::on_start_element(
  const Glib::ustring& name, const AttributeList& attributes)
{
  if (name == "offer") {
    std::cout << name << " (";
    for (const auto& attr_pair : attributes) {
      try {
        std::cout << attr_pair.name;
      }
      catch (const Glib::ConvertError& e) {
        std::cerr << "feed_parser::on_start_element(): Exception caught while converting name for std::cout: " << e.what() << std::endl;
      }

      try {
        std::cout << " = " <<  attr_pair.value << ")" << std::endl;
      }
      catch (const Glib::ConvertError& e) {
        std::cerr << "feed_parser::on_start_element(): Exception caught while converting value for std::cout: " << e.what() << std::endl;
      }
    }

    m_offer_root = std::make_shared<offer_node>();
    m_offer_root->name(name);
    m_current_offer_node = m_offer_root;
    m_offset = "  ";
  }
  else if (m_current_offer_node != nullptr) {
    //std::cout << "on_start_element: m_current_offer_node: " << m_offset << m_current_offer_node->name() << std::endl;
    offer_node_ptr new_node = std::make_shared<offer_node>();
    new_node->name(name);
    m_current_offer_node->add_child(new_node);
    m_current_offer_node = new_node;
    std::cout << "on_start_element: " << m_offset << m_current_offer_node->name() << std::endl;
    m_offset += "  ";
  }
}

void feed_parser::on_end_element(const Glib::ustring& name)
{
  if (name == "offer") {
    // TODO: add offer callback
    m_current_offer_node.reset();
    m_offer_root.reset();
    m_offset.clear();
    std::cout << "on_end_element: " << name << std::endl << std::endl;
    ::abort();
  }
  else if (m_current_offer_node != nullptr) {
    /*std::cout << m_offset << m_current_offer_node->name();
    if (!m_current_offer_node->data().empty()) {
      std::cout << ": " << m_current_offer_node->data();
    }
    std::cout << std::endl;*/
    m_current_offer_node = m_current_offer_node->parent();
    m_offset.erase(m_offset.end() - 2, m_offset.end());
  }
}

void feed_parser::on_characters(const Glib::ustring& text)
{
  if (m_current_offer_node != nullptr) {
    m_current_offer_node->data(text);
  }
}

void feed_parser::on_comment(const Glib::ustring& text)
{
  try {
    std::cout << "on_comment(): " << text << std::endl;
  }
  catch (const Glib::ConvertError& e) {
    std::cerr << "feed_parser::on_comment(): Exception caught while converting text for std::cout: " << e.what() << std::endl;
  }
}

void feed_parser::on_warning(const Glib::ustring& text)
{
  try {
    std::cout << "on_warning(): " << text << std::endl;
  }
  catch (const Glib::ConvertError& e) {
    std::cerr << "feed_parser::on_warning(): Exception caught while converting text for std::cout: " << e.what() << std::endl;
  }
}

void feed_parser::on_error(const Glib::ustring& text)
{
  try {
    std::cout << "on_error(): " << text << std::endl;
  }
  catch (const Glib::ConvertError& e) {
    std::cerr << "feed_parser::on_error(): Exception caught while converting text for std::cout: " << e.what() << std::endl;
  }
}

void feed_parser::on_fatal_error(const Glib::ustring& text)
{
  try {
    std::cout << "on_fatal_error(): " << text << std::endl;
  }
  catch (const Glib::ConvertError& e) {
    std::cerr << "feed_parser::on_characters(): Exception caught while converting value for std::cout: " << e.what() << std::endl;
  }
}

} // namespace realty
