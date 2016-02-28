#include "gtest/gtest.h"

#include "feed/feed.h"
#include "feed/bson.h"

void test_offer_callback(realty::feed::offer_node&& offer)
{
  std::cout << "test_offer_callback: " << offer.data() << std::endl;
  if (offer["type"] != Glib::ustring("продажа")) {
    std::cerr << "type: " << Glib::ustring(offer["type"]) << std::endl;
    throw std::runtime_error("Invalid offer type");
  }
  if (offer["price"]["currency"] != Glib::ustring("RUR")) {
    std::cerr << "price.currency: " << Glib::ustring(offer["price"]["currency"]) << std::endl;
    throw std::runtime_error("Invalid offer price.currency");
  }
}

TEST(feed_test, parse_file)
{
  EXPECT_NO_THROW(realty::feed::parse_feed_file("test.xml", realty::feed::to_bson));
  FAIL();
}
