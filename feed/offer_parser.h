#ifndef REALTY_FEED_OFFER_PARSER_H
#define REALTY_FEED_OFFER_PARSER_H

#include "feed/offer_node.h"

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include <mutex>

namespace realty {
namespace feed {

bsoncxx::document::value bson_from_offer(
  offer_node&& offer, bool only_sales = true, bool only_flats = true);

class offer_parser final
{
public:
  offer_parser(
    mongocxx::collection& collection, bool only_sales = true,
    bool only_flats = true);
  void parse(offer_node&& offer);
  uint64_t count() const { return m_count; }

private:
  uint64_t m_count;
  mongocxx::collection& m_collection;
  bool m_only_sales;
  bool m_only_flats;
  std::mutex m_mutex;
};

} // namespace feed
} // namespace realty

#endif // REALTY_FEED_OFFER_PARSER_H
