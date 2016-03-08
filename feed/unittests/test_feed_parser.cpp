#include "gtest/gtest.h"

#include "feed/feed.h"
#include "feed/offer_parser.h"

using namespace std::placeholders;

void test_cb(realty::feed::offer_node&&)
{
  // Empty
}

TEST(feed_test, parse_file)
{
  try {
    realty::feed::parse_feed_file("test.xml", test_cb);
  }
  catch (const std::exception& e) {
    FAIL() << e.what();
  }
}
