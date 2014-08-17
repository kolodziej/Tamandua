#include "user.hpp"
#include "message_buffer.hpp"
#include <utility>
#include <iostream>

using namespace tamandua;

void user::read_message()
{
	read_message_header_();
}

void user::deliver_message(const message &message)
{
	bool start_sending = messages_queue_.empty();
	messages_queue_.push_back(std::move(message));
	if (start_sending)
		send_messages_();
}

void user::read_message_header_()
{
	boost::asio::async_read(socket_,
		boost::asio::buffer(reinterpret_cast<char*>(&(read_message_.header)), sizeof(message_header)),
		[this](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				read_message_body_();
			}
		});
}

void user::read_message_body_()
{
	std::shared_ptr<char> buffer(new char[read_message_.header.size]);
	boost::asio::async_read(socket_,
		boost::asio::buffer(buffer.get(), read_message_.header.size),
		[this, buffer](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				read_message_.body = std::string(buffer.get(), read_message_.header.size);
				std::cout << read_message_.body << "\n";
				read_message();
			}
		});
}

void user::send_messages_()
{
	message_buffer buffer(messages_queue_.front().header, messages_queue_.front().body);
	boost::asio::async_write(socket_,
		boost::asio::buffer(buffer.get_buffer().get(), buffer.get_buffer_size()),
		[this](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				messages_queue_.pop_front();
				if (messages_queue_.empty() == false)
					send_messages_();
			}
		});
}
