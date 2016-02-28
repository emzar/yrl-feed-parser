#include "gtest/gtest.h"

#include "feed.h"

void test_offer_callback(realty::feed::offer_node_ptr&& offer)
{
  std::cout << "test_offer_callback: " << offer->data() << std::endl;
}

/*TEST(feed_test, stub)
{
  realty::feed::parse_feed_file(
    "/tmp/novostrojkispb.xml", test_offer_callback);
  FAIL() << "stub";
}*/

TEST(feed_test, url)
{
  realty::feed::parse_feed_url(
    "http://panfile.nichost.ru/reklama/novostrojkispb.xml",
    test_offer_callback);
  FAIL() << "stub";
}
