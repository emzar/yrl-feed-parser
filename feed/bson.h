#ifndef REALTY_FEED_BSON_H
#define REALTY_FEED_BSON_H

#include "feed/offer_node.h"

namespace realty {
namespace feed {

void to_bson(realty::feed::offer_node&& offer);

} // namespace feed
} // namespace realty

#endif // REALTY_FEED_BSON_H
