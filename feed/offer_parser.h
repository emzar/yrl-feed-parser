#ifndef REALTY_FEED_OFFER_PARSER_H
#define REALTY_FEED_OFFER_PARSER_H

#include "feed/offer_node.h"

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include <mutex>

namespace realty {
namespace feed {

class offer_parser final
{
public:
  offer_parser(mongocxx::collection& collection);
  void parse(offer_node&& offer);
  uint64_t count() const { return m_count; }

private:
  uint64_t m_count;
  mongocxx::collection& m_collection;
  std::mutex m_mutex;
};

} // namespace feed
} // namespace realty

#endif // REALTY_FEED_OFFER_PARSER_H
