# 概要

基底クラスの異なるboost::asio::ip::tcp::socketと
boost::asio::ssl::stream&lt;boost::asio::ip::tcp::socket&gt;を多態的に扱うためのWrapperクラス(のサンプル実装)

# やっていること

SyncStream要件を満す基底クラス(sync_stream_wrapper)を作成し、
boost::asio::ip::tcp::socketと
boost::asio::ssl::stream&lt;boost::asio::ip::tcp::socket&gt;をそれぞれWrapしたサブクラスtcp_socketとssl_streamを作成しています。

tcp_socketとssl_streamは多態的に扱うことができ、またSyncStream要件を満たすのでboost::asio::ip::tcp::socketなどと同じようにasio::read(),asio::write()などに渡すことができます。

以下の例(polymorphism.ccより抜粋)ではhttp_get()関数はsync_stream_wrapper型でソケットを受けるので、非TLSのtcp_socketとTLSのssl_streamを受け取ることができ、また、それらをそのままasio::read()/write()に渡すことができています。

<pre>
void http_get(sync_stream_wrapper&amp; sync_stream, const std::string&amp; host, const std::string&amp; path) {
  std::ostringstream request_stream;
  request_stream &lt;&lt; &quot;GET &quot; &lt;&lt; path &lt;&lt; &quot; HTTP/1.1\r\n&quot;
                 &lt;&lt; &quot;Host: &quot; &lt;&lt; host &lt;&lt; &quot;\r\n&quot;
                 &lt;&lt; &quot;Connection: Close\r\n&quot;;
  request_stream &lt;&lt; &quot;\r\n&quot;;

  // sync_stream_wrapper mets SyncStream requirements and
  // can be passed to asio::write().
  <span style="color:blue;">asio::write(sync_stream, asio::buffer(request_stream.str()));</span>

  // We can also pass asio::streambuf(DynamicBuffer requirements) to
  // asio::read().
  boost::system::error_code error;
  asio::streambuf receive_buffer;

  <span style="color:blue;">asio::read(sync_stream, receive_buffer, error);</span>
  if (error &amp;&amp; error != asio::error::eof) {
    throw std::runtime_error(error.message());
  }
  std::cout &lt;&lt; asio::buffer_cast&lt;const char*&gt;(receive_buffer.data()) &lt;&lt; std::endl;
}
</pre>
