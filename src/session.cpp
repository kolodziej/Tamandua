#include "session.hpp"

using namespace tamandua;

session::session(server &svr, boost::asio::ssl::context &ctx) :
	server_(svr),
	ssl_stream_(svr.get_io_stream(), ctx)
{}

void session::start(std::function<void()> handshake_callback, std::function<void(message&)> message_received)
{
	perform_handshake_(handshake_callback);
}

ssl_socket_stream& session::get_ssl_stream()
{
	return ssl_stream_;
}

ssl_socket_stream::lowest_layer_type& session::get_socket()
{
	return ssl_stream_.lowest_layer();
}

std::string session::get_ip_address()
{
	return get_socket().remote_endpoint().address().to_string();
}

message& session::read_message()
{
	read_message_header_();
}

server& session::get_server()
{
	return server_;
}

void session::perform_handshake_(std::function<void()> callback)
{
	ssl_stream_.async_handshake(boost::asio::ssl::stream_base::server,
	[this, &callback](boost::system::error_code ec)
	{
		if (!ec)
			callback();
		else
			Error(get_server().get_logger(), "Error while handshake: ", ec.message());
	});
}

void session::read_message_header_()
{
	boost::asio::async_read(ssl_stream_,
		boost::asio::buffer(reinterpret_cast<char*>(&read_message_.header), sizeof(read_message_.header)),
		[this](boost::asio::error_code ec, size_t length)
		{
			if (!ec)
				read_message_body_();
			else
				Error(get_server().get_logger(), "Error while reading message's header: ", ec.message());
		});
}

void session::read_message_body_()
{
	std::unique_ptr<char> buffer(new char[read_message_.header.size]);
	boost::asio::async_read(ssl_socket_,
		boost::asio::buffer(buffer.get(), read_message_.header.size),
		[this](boost::system::error_code ec, size_t length)
		{
			if (!ec)
				message_received_callback_();
			else
				Error(get_server().get_logger(), "Error while receiving message: ", ec.message());
		});
}
