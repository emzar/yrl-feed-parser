#include "gtest/gtest.h"

#include "feed/feed.h"
#include "feed/offer_parser.h"

TEST(feed_test, parse_file)
{
  try {
    realty::feed::parse_feed_file("test.xml", realty::feed::parse_offer);
  }
  catch (const std::exception& e) {
    FAIL() << e.what();
  }
}
