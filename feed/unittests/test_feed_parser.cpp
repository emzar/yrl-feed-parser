#include "gtest/gtest.h"

#include "parser.h"

TEST(feed_test, stub)
{
  realty::feed::parse_feed("/tmp/novostrojkispb.xml");
  FAIL() << "stub";
}
