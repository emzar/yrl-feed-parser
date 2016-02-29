#include "offer_parser.h"

#include <algorithm>
#include <unordered_map>
#include <set>

namespace realty {
namespace feed {

static const std::unordered_multimap<std::string, Glib::ustring>
valid_offer_elements = {{"type", "продажа"}, {"category", "квартира"}};

static const std::set<Glib::ustring> valid_price_currency = {"rub", "rur"};

namespace {

void check_elements(realty::feed::offer_node& offer)
{
  for (const auto& valid_offer_element : valid_offer_elements) {
    if (offer[valid_offer_element.first] != valid_offer_element.second) {
      throw std::runtime_error("Invalid " + valid_offer_element.first);
    }
  }
}

void check_currency(realty::feed::offer_node& offer)
{
  const auto& currency = Glib::ustring(offer["price"]["currency"]).lowercase();
  if (valid_price_currency.find(currency) == valid_price_currency.end()) {
    throw std::runtime_error("Invalid price currency");
  }
}

} // anonymous namespace

void parse_offer(realty::feed::offer_node&& offer)
{
  check_elements(offer);
  check_currency(offer);
}

} // namespace feed
} // namespace realty
