#include "gtest/gtest.h"

#include "feed_parser.h"

TEST(feed_test, stub)
{
  realty::parse_feed("/tmp/novostrojkispb.xml");
  FAIL() << "stub";
}
