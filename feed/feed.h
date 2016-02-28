#ifndef REALTY_FEED_FEED_H
#define REALTY_FEED_FEED_H

#include <string>

namespace realty {
namespace feed {

void parse_feed_url(const std::string& url);
void parse_feed_file(const std::string& filename);

} // namespace feed
} // namespace realty

#endif // REALTY_FEED_FEED_H
