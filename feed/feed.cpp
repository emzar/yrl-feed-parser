#include "feed.h"

#include <cstring>
#include <fstream>

namespace realty {
namespace feed {

static const size_t PARSE_FEED_BUFFER_SIZE = 4096;

void parse_feed_url(
  const std::string& /*url*/, fn_offer_callback&& /*offer_callback*/)
{
  // Empty
}

void parse_feed_file(
  const std::string& filename, fn_offer_callback&& offer_callback)
{
  parse_feed_stream(std::ifstream(filename), std::move(offer_callback));
}

void parse_feed_stream(std::istream&& is, fn_offer_callback&& offer_callback)
{
  char buffer[PARSE_FEED_BUFFER_SIZE];
  realty::feed::parser parser(std::move(offer_callback));
  parser.set_substitute_entities(true);
  do {
    std::memset(buffer, 0, PARSE_FEED_BUFFER_SIZE);
    is.read(buffer, PARSE_FEED_BUFFER_SIZE - 1);
    if (is.gcount()) {
      // We use Glib::ustring::ustring(InputIterator begin, InputIterator end)
      // instead of Glib::ustring::ustring( const char*, size_type ) because it
      // expects the length of the string in characters, not in bytes.
      Glib::ustring input(buffer, buffer + is.gcount());
      parser.parse_chunk(input);
    }
  }
  while (is);
}

} // namespace feed
} // namespace realty
