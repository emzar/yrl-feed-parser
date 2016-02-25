#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;

int main(int, char**)
{
  mongocxx::instance inst{};
  mongocxx::client conn{mongocxx::uri{}};
  auto agents = conn["realty"]["agents"];

  bsoncxx::builder::stream::document filter_builder;

  filter_builder << "$and"
    << open_array
    << open_document << "feed_url" << open_document << "$ne" << "" << close_document
      << close_document
    << open_document << "ignore_feed" << false << close_document
    << close_array;

  auto cursor = agents.find(filter_builder.view());
  for (auto&& doc : cursor) {
    std::cout << bsoncxx::to_json(doc) << std::endl;
  }
  return 0;
}
