#include "gtest/gtest.h"

#include "feed/feed.h"
#include "feed/offer_parser.h"

#include <bsoncxx/json.hpp>

using namespace std::placeholders;

void test_cb(realty::feed::offer_node&& offer)
{
  try {
    std::cout << "\noffer:\n"
      << bsoncxx::to_json(bson_from_offer(std::move(offer))) << std::endl;
  }
  catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}

TEST(feed_test, parse_file)
{
  try {
    realty::feed::parse_feed_file("test.xml", test_cb);
    FAIL();
  }
  catch (const std::exception& e) {
    FAIL() << e.what();
  }
}
