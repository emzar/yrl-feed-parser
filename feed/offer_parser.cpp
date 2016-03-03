#include "offer_parser.h"

#include <algorithm>
#include <boost/date_time.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <unordered_map>
#include <set>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;

namespace bt = boost::posix_time;

namespace realty {
namespace feed {

static const std::unordered_multimap<std::string, Glib::ustring>
valid_offer_elements = {{"type", "продажа"}, {"category", "квартира"}};

static const std::set<Glib::ustring> valid_price_currency = {"rub", "rur"};

namespace {

static const auto format = std::locale(
  std::locale::classic(), new bt::time_input_facet("%Y-%m-%dT%H:%M:%S"));

bool is_expired(realty::feed::offer_node& offer)
{
  std::istringstream is(offer["expire-date"].data().raw());
  is.imbue(format);
  bt::ptime expire_date;
  is >> expire_date;
  bt::ptime now = bt::second_clock::local_time();
  std::cout << "expire_date: " << expire_date << std::endl;
  std::cout << "now:         " << now << std::endl;
  std::cout << "expired:     " << (now > expire_date) << std::endl;
  return (now > expire_date);
}

void check_currency(realty::feed::offer_node& offer)
{
  const auto& currency = Glib::ustring(offer["price"]["currency"]).lowercase();
  if (valid_price_currency.find(currency) == valid_price_currency.end()) {
    throw std::runtime_error("Invalid price currency");
  }
}

void check_elements(realty::feed::offer_node& offer)
{
  if (is_expired(offer)) {
    throw std::runtime_error("Offer is expired");
  }
  for (const auto& valid_offer_element : valid_offer_elements) {
    if (offer[valid_offer_element.first] != valid_offer_element.second) {
      throw std::runtime_error("Invalid " + valid_offer_element.first);
    }
  }
  check_currency(offer);
  if (offer["yandex-building-id"].data().empty()) {
    throw std::runtime_error("Invalid yandex-building-id");
  }
}

void parse_offer(realty::feed::offer_node& offer_node, document& doc_stream)
{
  if (offer_node.has_children()) {
    doc_stream << offer_node.name() << open_document;
    for (offer_children_type::const_reference child : offer_node.children()) {
      parse_offer(*child.second, doc_stream);
    }
    doc_stream << close_document;
  }
  else {
    doc_stream << offer_node.name() << offer_node.data();
  }
}

} // anonymous namespace

void parse_offer(realty::feed::offer_node&& offer)
{
  check_elements(offer);
  document doc_stream = document{};
  parse_offer(offer, doc_stream);
  bsoncxx::document::value doc = doc_stream << finalize;
  std::cout << bsoncxx::to_json(doc) << std::endl;

  throw std::exception();
}

} // namespace feed
} // namespace realty
