#ifndef REALTY_FEED_FEED_H
#define REALTY_FEED_FEED_H

#include "feed/parser.h"
#include <istream>

namespace realty {
namespace feed {

void parse_feed_url(const std::string& url, const std::string& feed_id, fn_offer_callback&& cb);
void parse_feed_file(const std::string& filename, fn_offer_callback&& cb);
void parse_feed_stream(std::istream&& is, fn_offer_callback&& cb);

} // namespace feed
} // namespace realty

#endif // REALTY_FEED_FEED_H
