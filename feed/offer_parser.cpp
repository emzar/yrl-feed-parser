#include "offer_parser.h"

#include <algorithm>
#include <boost/date_time.hpp>
#include <iostream>
#include <unordered_map>
#include <set>

namespace bt = boost::posix_time;

namespace realty {
namespace feed {

static const std::unordered_multimap<std::string, Glib::ustring>
valid_offer_elements = {{"type", "продажа"}, {"category", "квартира"}};

static const std::set<Glib::ustring> valid_price_currency = {"rub", "rur"};

namespace {

static const bt::ptime start_epoch(boost::gregorian::date(1970, 1, 1));
static const auto format = std::locale(
  std::locale::classic(), new bt::time_input_facet("%Y-%m-%dT%H:%M:%S"));

bt::time_duration::sec_type seconds_from_epoch(const std::string& str)
{
  std::istringstream is(str);
  is.imbue(format);
  bt::ptime pt;
  is >> pt;
  return (pt - start_epoch).total_seconds();
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

} // anonymous namespace

void parse_offer(realty::feed::offer_node&& offer)
{
  check_elements(offer);
  std::cout << "expire-date: " << offer["expire-date"].data().raw() << std::endl;
  std::cout << "seconds_from_epoch: " << seconds_from_epoch(offer["expire-date"].data().raw()) << std::endl;
  throw std::exception();
}

} // namespace feed
} // namespace realty
