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

} // anonymous namespace

// yandex­-building-­id // (унифицировать не надо)
// price­ // пишем в базу в рублях (может быть в валюте или цене за кв.м)
// area // общая площадь, пишем в кв. м
// living­-space // жилая площадь, пишем в кв. м
// kitchen­-area // площадь кухни, пишем в кв. м
// renovation // отделка, ее нужно унифицировать до значений: “без отделки”, “под чистовую” и “полная” //сейчас в фидах только такие варианты, но надо заложить место, куда можно будет дописывать алиасы
// image // планировка
// rooms // кол­во комнат
// bathroom­-unit // тип с/у
// floor // этаж
// floors-­total // всего этажей в этой парадной
// building-­type // тип стен
// building­=state // стадия строительства
// built­-year // год сдачи
// ready­-quarter // квартрал сдачи
// building-­phase // очередь
// building­-section // корпус
// ceiling­-height // высота потолков

void parse_offer(realty::feed::offer_node&& offer)
{
  check_elements(offer);

  //std::cout << "yandex-building-id: " << offer["yandex-building-id"].data() << std::endl;
  //std::cout << "price: " << offer["price"]["value"].data() << std::endl;
  //std::cout << "area: " << offer["area"]["value"].data() << std::endl;
  //std::cout << "living-space: " << offer["living-space"]["value"].data() << std::endl;
  //std::cout << "kitchen-space: " << offer["kitchen-space"]["value"].data() << std::endl;
  //std::cout << "renovation: " << offer["renovation"].data() << std::endl;
  //std::cout << "image: " << offer["image"].data() << std::endl;
  //std::cout << "rooms: " << offer["rooms"].data() << std::endl;
  //std::cout << "bathroom-unit: " << offer["bathroom-unit"].data() << std::endl;
  //std::cout << "floor: " << offer["floor"].data() << std::endl;
  //std::cout << "floors-total: " << offer["floors-total"].data() << std::endl;
  //std::cout << "building-type: " << offer["building-type"].data() << std::endl;
  //std::cout << "building-state: " << offer["building-state"].data() << std::endl;
  //std::cout << "built-year: " << offer["built-year"].data() << std::endl;
  //std::cout << "ready-quarter: " << offer["ready-quarter"].data() << std::endl;
  //std::cout << "building-phase: " << offer["building-phase"].data() << std::endl;
  //std::cout << "building-section: " << offer["building-section"].data() << std::endl;
  //std::cout << "ceiling-height: " << offer["ceiling-height"].data() << std::endl;

  throw std::exception();
}

} // namespace feed
} // namespace realty
