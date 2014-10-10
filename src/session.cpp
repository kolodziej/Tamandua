#include "session.hpp"
#include "server.hpp"
#include "user.hpp"
#include "message_buffer.hpp"

using namespace tamandua;

session::session(server &svr, boost::asio::ssl::context &ctx) :
	server_(svr),
	ssl_stream_(svr.get_io_service(), ctx)
{}

void session::start()
{
	perform_handshake_();
}

void session::stop()
{

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

void session::read_message()
{
	read_message_header_();
}

server& session::get_server()
{
	return server_;
}

void session::perform_handshake_()
{
	ssl_stream_.async_handshake(boost::asio::ssl::stream_base::server,
	[this](boost::system::error_code ec)
	{
		if (!ec)
			read_message();
		else
			Error(get_server().get_logger(), "Error while handshake: ", ec.message());
	});
}

void session::read_message_header_()
{
	boost::asio::async_read(ssl_stream_,
		boost::asio::buffer(reinterpret_cast<char*>(&read_message_.header), sizeof(read_message_.header)),
		[this](boost::system::error_code ec, size_t length)
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
	boost::asio::async_read(ssl_stream_,
		boost::asio::buffer(buffer.get(), read_message_.header.size),
		[this](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				get_server().process_message(nullptr, read_message_);
				read_message_header_();
			} else
				Error(get_server().get_logger(), "Error while receiving message: ", ec.message());
		});
}

void session::send_messages_()
{
	message_buffer buffer(messages_queue_.front().header, messages_queue_.front().body);
	boost::asio::async_write(ssl_stream_,
		boost::asio::buffer(buffer.get_buffer().get(), buffer.get_buffer_size()),
		[this](boost::system::error_code ec, size_t length)
		{
			if (ec == boost::system::errc::operation_canceled)
				return;

			if (!ec)
			{
				messages_queue_.pop_front();
				if (messages_queue_.empty() == false)
					send_messages_();
			} else
			{
				//Error(get_server().get_logger(), "An error occurred while sending message to user ", get_name(), " (ID: ", get_id(), "): ", ec.message());
			}
		});
}
