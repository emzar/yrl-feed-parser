#include "gtest/gtest.h"

#include "feed/feed.h"
#include "feed/offer_parser.h"

using namespace std::placeholders;

TEST(feed_test, parse_file)
{
  try {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};
    auto db = conn["realty"];
    std::cout << "connected\n";

    realty::feed::parse_feed_file(
      "test.xml", std::bind(realty::feed::parse_offer, _1, std::ref(db)));
  }
  catch (const std::exception& e) {
    FAIL() << e.what();
  }
}
