#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <curl/curl.h>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <thread>
#include <vector>

#include "feed/feed.h"
#include "feed/offer_parser.h"

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;

using namespace std::placeholders;

std::string get_str_value(
  const bsoncxx::v_noabi::document::view& doc, const std::string& key)
{
  auto value = bsoncxx::to_json(doc[key].get_value());
  // remove first and last '"'
  value = value.substr(1, value.size() - 2);
  return value;
}

int main(int, char**)
{
  mongocxx::instance inst{};
  mongocxx::client conn{mongocxx::uri{}};
  auto db = conn["realty"];

  // remove all offers
  db["offers"].delete_many({});

  bsoncxx::builder::stream::document filter_builder;
  filter_builder << "$and"
    << open_array
    << open_document
      << "feed_url" << open_document << "$ne" << "" << close_document
    << close_document
    << open_document << "ignore_feed" << false << close_document
    << close_array;

  curl_global_init(CURL_GLOBAL_ALL);
  auto agents = db["agents"];
  auto cursor = agents.find(filter_builder.view());
  std::vector<std::thread> workers;
  for (auto&& doc : cursor) {
    //std::cout << bsoncxx::to_json(doc) << std::endl;
    auto identifier = get_str_value(doc, "identifier");
    auto download_filename = "/tmp/agency_" + identifier;
    auto feed_url = get_str_value(doc, "feed_url");
    workers.push_back(std::thread(
      realty::feed::parse_feed_url, std::move(feed_url),
      std::bind(realty::feed::parse_offer, _1, std::ref(db))));
  }

  for (auto& worker : workers) {
    worker.join();
  }

  return 0;
}
