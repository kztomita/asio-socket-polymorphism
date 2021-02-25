#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include "sync_stream_wrapper.h"

namespace asio = boost::asio;

void http_get(sync_stream_wrapper& sync_stream, const std::string& host, const std::string& path) {
  std::ostringstream request_stream;
  request_stream << "GET " << path << " HTTP/1.1\r\n"
                 << "Host: " << host << "\r\n"
                 << "Connection: Close\r\n";
  request_stream << "\r\n";

  // sync_stream_wrapper mets SyncStream requirements and
  // can be passed to asio::write().
  asio::write(sync_stream, asio::buffer(request_stream.str()));

  // We can also pass asio::streambuf(DynamicBuffer requirements) to
  // asio::read().
  boost::system::error_code error;
  asio::streambuf receive_buffer;

  asio::read(sync_stream, receive_buffer, error);
  if (error && error != asio::error::eof) {
    throw std::runtime_error(error.message());
  }
  std::cout << asio::buffer_cast<const char*>(receive_buffer.data()) << std::endl;
}

int main() {
  const std::string host = "localhost";
  asio::io_context io_context;
  asio::ip::tcp::resolver resolver(io_context);
  auto endpoints = resolver.resolve(asio::ip::tcp::v4(), host, "");
  auto ip = endpoints.begin()->endpoint().address();

  {
    // wrapper of boost::asio::ip::tcp::socket
    tcp_socket s;
    s.connect(ip, 80);
    http_get(s, host, "/");
  }
  {
    // wrapper of boost::asio::ssl::stream<boost::asio::ip::tcp::socket>
    ssl_stream s(false);
    s.connect(ip, 443);
    http_get(s, host, "/");
  }

  return 0;
}
