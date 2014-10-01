#include "session.hpp"

session::session(boost::asio::io_service &io, boost::asio::ssl::context &ctx) :
	ssl_stream_(io, ctx)
{}

void session::start()
{

}
