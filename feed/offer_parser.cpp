#include "offer_parser.h"

#include <algorithm>
#include <boost/date_time.hpp>
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
namespace bt = boost::posix_time;

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::finalize;

namespace realty {
namespace feed {

namespace {

static const auto format = std::locale(
  std::locale::classic(), new bt::time_input_facet("%Y-%m-%dT%H:%M:%S"));

bool is_expired(realty::feed::offer_node& offer)
{
  /*std::istringstream is(offer["expire-date"].data().raw());
  is.imbue(format);
  bt::ptime expire_date;
  is >> expire_date;
  bt::ptime now = bt::second_clock::local_time();
  std::cout << "expire_date: " << expire_date << std::endl;
  std::cout << "now:         " << now << std::endl;
  std::cout << "expired:     " << (now > expire_date) << std::endl;
  return (now > expire_date);*/
  return true;
}

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
    doc_stream << node.name() << open_document;
    for (offer_children_type::const_reference child : node.children()) {
      parse_offer(*child, doc_stream);
    }
    doc_stream << close_document;
  }
  else {
    if (try_add<long int>(node, doc_stream)) return;
    if (try_add<double>(node, doc_stream)) return;
    doc_stream << node.name() << node.data();
  }
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
