#include "offer_parser.h"

#include <boost/lexical_cast.hpp>
#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

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
bool try_add(offer_node& node, document& doc_stream)
{
  try {
    doc_stream << node.name() << lexical_cast<T>(node.data());
  }
  catch (const bad_lexical_cast&) {
    return false;
  }
  return true;
}

void parse_offer(offer_node& node, document& doc_stream)
{
  if (node.has_children()) {
    if (node.name() != "offer") doc_stream << node.name() << open_document;
    for (offer_children_type::const_reference child : node.children()) {
      parse_offer(*child, doc_stream);
    }
    if (node.name() != "offer") doc_stream << close_document;
    return;
  }
  if (try_add<long int>(node, doc_stream)) return;
  if (try_add<double>(node, doc_stream)) return;
  if (node.data() == "true") doc_stream << node.name() << true;
  else if (node.data() == "false") doc_stream << node.name() << false;
  else doc_stream << node.name() << node.data();
}

} // anonymous namespace

void parse_offer(offer_node&& offer)
{
  document doc_stream = document{};
  parse_offer(offer, doc_stream);
  bsoncxx::document::value doc = doc_stream << finalize;
  std::cout << bsoncxx::to_json(doc) << std::endl;

  throw std::exception();
}

} // namespace feed
} // namespace realty
