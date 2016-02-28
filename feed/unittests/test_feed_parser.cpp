#include "gtest/gtest.h"

#include "feed/feed.h"

void test_offer_callback(realty::feed::offer_node_ptr&& offer)
{
  std::cout << "test_offer_callback: " << offer->data() << std::endl;
}

TEST(feed_test, parse_file)
{
  EXPECT_NO_THROW(
    realty::feed::parse_feed_file("test1.xml", test_offer_callback));
}
