#include "offer_parser.h"

#include <boost/lexical_cast.hpp>
#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>

using boost::lexical_cast;
using boost::bad_lexical_cast;

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::finalize;

namespace realty {
namespace feed {

namespace {

template<typename T>
bool try_add(const std::string& name, offer_node& node, document& doc_stream)
{
  try {
    doc_stream << name << lexical_cast<T>(node.data());
  }
  catch (const bad_lexical_cast&) {
    return false;
  }
  return true;
}

void parse_offer(
  offer_node& node, document& doc_stream, bool only_sales, bool only_flats)
{
  std::string name = node.name().raw();
  std::replace(name.begin(), name.end(), '-', '_');

  // ignore non-sales
  if (only_sales && (node.name() == "type") && (node.data() != "продажа")) {
    throw std::runtime_error("invalid type: " + node.data());
  }

  // ignore non-flats
  if (only_flats && (node.name() == "category") && (node.data() != "квартира") &&
    (node.parent() != nullptr) && (node.parent()->name() == "offer")) {
    throw std::runtime_error("invalid category: " + node.data());
  }

  if (node.has_children()) {
    if (node.name() != "offer") doc_stream << name << open_document;
    for (offer_children_type::const_reference child : node.children()) {
      parse_offer(*child, doc_stream, only_sales, only_flats);
    }
    if (node.name() != "offer") doc_stream << close_document;
    return;
  }
  if (try_add<int>(name, node, doc_stream)) return;
  if (try_add<double>(name, node, doc_stream)) return;
  if (node.data() == "true") doc_stream << name << true;
  else if (node.data() == "false") doc_stream << name << false;
  else doc_stream << name << node.data();
}

} // anonymous namespace

bsoncxx::document::value bson_from_offer(
  offer_node&& offer, bool only_sales, bool only_flats)
{
  document doc_stream = document{};
  parse_offer(offer, doc_stream, only_sales, only_flats);
  return (doc_stream << finalize);
}

offer_parser::offer_parser(
  mongocxx::collection& collection, bool only_sales, bool only_flats)
  : m_count(0),
    m_collection(collection),
    m_only_sales(only_sales),
    m_only_flats(only_flats)
{
  // Empty
}

void offer_parser::parse(offer_node&& offer)
{
  try {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_collection.insert_one(
      std::move(bson_from_offer(std::move(offer), m_only_sales, m_only_flats)));
    if (((m_count % 10000) == 0) && (m_count != 0)) {
      std::cout << "Handeled " << m_count << " offers\n";
    }
    m_count++;
  }
  catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}

} // namespace feed
} // namespace realty
