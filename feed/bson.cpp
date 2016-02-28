#include "bson.h"
#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;

namespace realty {
namespace feed {

static const Glib::ustring valid_type = "продажа";

void check_type(realty::feed::offer_node& offer)
{
  if (offer["type"] != valid_type) {
    throw std::runtime_error("Invalid type");
  }
}

void to_bson(realty::feed::offer_node&& offer)
{
  check_type(offer);

  bsoncxx::document::value restaurant_doc =
    document{} << "address" << open_document << "street"
               << "2 Avenue"
               << "zipcode"
               << "10075"
               << "building"
               << "1480"
               << "coord" << open_array << -73.9557413 << 40.7720266 << close_array
               << close_document << "borough"
               << "Manhattan"
               << "cuisine"
               << "Italian"
               << "grades" << open_array << open_document << "date"
               << bsoncxx::types::b_date{12323} << "grade"
               << "A"
               << "score" << 11 << close_document << open_document << "date"
               << bsoncxx::types::b_date{121212} << "grade"
               << "B"
               << "score" << 17 << close_document << close_array << "name"
               << "Vella"
               << "restaurant_id"
               << "41704620" << finalize;
  std::cout << bsoncxx::to_json(restaurant_doc) << std::endl;

  document doc_stream = document{};
  doc_stream << "type" << Glib::ustring(offer["type"]).raw();
  bsoncxx::document::value doc = doc_stream << finalize;
  std::cout << bsoncxx::to_json(doc) << std::endl;
}

} // namespace feed
} // namespace realty
