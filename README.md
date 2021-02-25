# 概要

基底クラスの異なるboost::asio::ip::tcp::socketと
boost::asio::ssl::stream<boost::asio::ip::tcp::socket>を多態的(動的ポリモーフィズム)に扱うためのWrapperクラス(のサンプル実装)

# やっていること

SyncStream要件を満す基底クラスを作成し、
boost::asio::ip::tcp::socketと
boost::asio::ssl::stream<boost::asio::ip::tcp::socket>をWrapしたサブクラスを作成しこれらにread/writeの処理

boost::asio::ip::tcp::socket