#ifndef REALTY_FEED_OFFER_PARSER_H
#define REALTY_FEED_OFFER_PARSER_H

#include "feed/offer_node.h"

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

namespace realty {
namespace feed {

void parse_offer(
  offer_node&& offer, mongocxx::database& db);

} // namespace feed
} // namespace realty

#endif // REALTY_FEED_OFFER_PARSER_H
