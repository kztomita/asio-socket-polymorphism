#include "sync_stream_wrapper.h"
#include <openssl/ssl.h>

namespace asio = boost::asio;

tcp_socket::tcp_socket()
  : socket_(io_context_) {
}

void tcp_socket::set_host(const std::string& host) {
}

void tcp_socket::connect(const boost::asio::ip::address& ip, uint16_t port) {
  socket_.connect(asio::ip::tcp::endpoint(ip, port));
}

std::size_t tcp_socket::read_some(const boost::asio::mutable_buffer& buffer) {
  return socket_.read_some(buffer);
}
std::size_t tcp_socket::read_some(const boost::asio::mutable_buffer& buffer, boost::system::error_code& ec) {
  return socket_.read_some(buffer, ec);
}

std::size_t tcp_socket::write_some(const boost::asio::const_buffer& buffer) {
  return socket_.write_some(buffer);
}
std::size_t tcp_socket::write_some(const boost::asio::const_buffer& buffer, boost::system::error_code& ec) {
  return socket_.write_some(buffer, ec);
}


ssl_stream::ssl_stream()
  : ssl_stream(true) {
}

ssl_stream::ssl_stream(bool verify_cert)
  : ssl_context_(asio::ssl::context::tlsv12_client),
    verify_cert_(verify_cert),
    host_("localhost") {

  ssl_context_.set_default_verify_paths();

  stream_.reset(new socket_type(io_context_, ssl_context_));
}

void ssl_stream::set_host(const std::string& host) {
  host_ = host;
}

void ssl_stream::connect(const boost::asio::ip::address& ip, uint16_t port) {
  stream_->lowest_layer().connect(asio::ip::tcp::endpoint(ip, port));

  if (verify_cert_) {
    // for SNI
    SSL_set_tlsext_host_name(stream_->native_handle(), host_.c_str());

    stream_->set_verify_mode(asio::ssl::verify_peer);
    stream_->set_verify_callback(asio::ssl::rfc2818_verification(host_));
  }

  stream_->handshake(asio::ssl::stream_base::client);
}

std::size_t ssl_stream::read_some(const boost::asio::mutable_buffer& buffer) {
  return stream_->read_some(buffer);
}
std::size_t ssl_stream::read_some(const boost::asio::mutable_buffer& buffer, boost::system::error_code& ec) {
  return stream_->read_some(buffer, ec);
}

std::size_t ssl_stream::write_some(const boost::asio::const_buffer& buffer) {
  return stream_->write_some(buffer);
}
std::size_t ssl_stream::write_some(const boost::asio::const_buffer& buffer, boost::system::error_code& ec) {
  return stream_->write_some(buffer, ec);
}

ssl_stream::socket_type& ssl_stream::inner_stream() const {
  return *stream_;
}

