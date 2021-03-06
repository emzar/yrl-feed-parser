#include "feed.h"

#include <cstring>
#include <ctime>
#include <curl/curl.h>
#include <fstream>
#include <iostream>

namespace realty {
namespace feed {

static const size_t PARSE_FEED_BUFFER_SIZE = 4096;

struct curl_user_data
{
  realty::feed::parser* parser;
  std::ofstream* ofs;
};

size_t with_parse_write_data(
  void* ptr, size_t size, size_t nmemb, curl_user_data* user_data)
{
  char* ptr_impl = static_cast<char*>(ptr);
  *(user_data->ofs) << ptr_impl;
  Glib::ustring input(ptr_impl, ptr_impl + size * nmemb);
  user_data->parser->parse_chunk(input);
  return size * nmemb;
}

std::string get_feed_path_postfix()
{
  time_t t = time(0);
  struct tm* now = localtime(&t);
  char buf[32];
  strftime(buf, sizeof(buf), "_feed_%Y_%m_%d_%H", now);
  return buf;
}

void parse_feed_url(
  const std::string& url, const std::string& feed_id, const std::string& feed_name,
  const std::string& storage_path, fn_offer_callback&& offer_callback)
{
  std::string path = storage_path + "/feed/" + feed_name.substr(0, 2) + '/';
  std::string cmd = "mkdir -p " + path;
  ::system(cmd.c_str());
  std::string feed_filename = path + feed_name + get_feed_path_postfix();
  std::ofstream ofs(feed_filename);

  realty::feed::parser parser(std::move(offer_callback), std::move(feed_id));
  parser.set_substitute_entities(true);

  curl_user_data user_data;
  user_data.parser = &parser;
  user_data.ofs = &ofs;

  CURL* curl = curl_easy_init();
  if (curl != nullptr) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, with_parse_write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &user_data);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
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
