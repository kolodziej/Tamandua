#include "user.hpp"
#include "message_buffer.hpp"
#include "message_composer.hpp"
#include "room.hpp"
#include "private_room.hpp"
#include <utility>
#include <iostream>
#include <cmath>

using namespace tamandua;

user::~user()
{
	Log(get_server().get_logger(), "User ID ", get_id(), " is leaving server!");
}

void user::start()
{
	perform_handshake_();
}

ssl_socket_stream::lowest_layer_type &user::get_socket()
{
	return socket_.lowest_layer();
}

std::string user::get_ip_address()
{
	return get_socket().remote_endpoint().address().to_string();
}

void user::read_message()
{
	if (quit_status_ != ok)
	{
		if (quit_status_ == user_quit)
			quit_();
		else if (quit_status_ == user_error_quit)
			error_quit_();

		return;
	}	
	read_message_header_();
}

void user::deliver_message(const message &message)
{
	if (quit_status_ != ok)
		return;

	bool start_sending = messages_queue_.empty();
	messages_queue_.push_back(message);
	if (start_sending)
		send_messages_();
}

void user::lock(unsigned int minutes, std::string reason)
{
	locked_until_ = message_time_clock_t::now() + std::chrono::minutes(minutes);
	message_composer msgc(warning_message);
	msgc << "You have been locked for " << minutes << " minutes. ";
	if (reason.empty() == false)
		msgc << "(reason: " << reason << ")";

	deliver_message(msgc());
}

void user::unlock(std::string reason)
{
	locked_until_ = message_time_clock_t::now() - std::chrono::seconds(1);
	message_composer msgc(info_message);
	msgc << "You have been unlocked! ";
	if (reason.empty() == false)
		msgc << "(reason: " << reason << ")";

	deliver_message(msgc());
}

bool user::is_locked()
{
	return (locked_until_ < message_time_clock_t::now());
}

void user::remove(std::string reason)
{
	message_composer msgc(warning_message);
	msgc << "You have been removed from server by root! ";
	if (reason.empty() == false)
		msgc << "(reason: " << reason << ")";

	deliver_message(msgc());
	quit_();
}

void user::quit()
{
	quit_status_ = user_quit;
}

void user::perform_handshake_()
{
	socket_.async_handshake(boost::asio::ssl::stream_base::server,
	[this](boost::system::error_code ec)
	{
		if (!ec)
		{
			get_server().send_startup_data(shared_from_this());
			add_to_hall_();
			read_message();
		} else
		{
			Error(get_server().get_logger(), "Handshake with user ID: ", get_id(), " failed. Error: ", ec.message());
			error_quit_();
		}
	});
}

void user::add_to_hall_()
{
	get_server().get_group(0)->join_participant(shared_from_this());
	add_group(0);
}

void user::read_message_header_()
{
	boost::asio::async_read(socket_,
		boost::asio::buffer(reinterpret_cast<char*>(&(read_message_.header)), sizeof(message_header)),
		[this](boost::system::error_code ec, size_t length)
		{
			if (ec == boost::system::errc::operation_canceled)
				return;

			if (!ec)
			{
				read_message_body_();
			} else
			{
				Error(get_server().get_logger(), "Error while reading header: ", ec.message());
				error_quit_();
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
			if (ec == boost::system::errc::operation_canceled)
				return;

			if (!ec)
			{
				read_message_.body = std::string(buffer.get(), read_message_.header.size);
				Log(get_server().get_logger(), "User ", get_id(), " passed a message: ", read_message_.body);
				process_message_();
			} else
			{
				Error(get_server().get_logger(), "User ", get_id(), " failed receiveg a message from client!");
				error_quit_();
			}
		});
}

void user::process_message_()
{
	if (read_message_.header.type == quit_message)
	{
		quit_();
	} else if (message_time_clock_t::now() < locked_until_)
	{
		message_composer msgc(warning_message, read_message_.header.group);
		msgc << "You have been locked! (~"
				<< ceil(std::chrono::duration_cast<std::chrono::duration<double, std::ratio<60,1>>>(locked_until_ - message_time_clock_t::now()).count())
				<< " minutes left)";
		deliver_message(msgc());
		read_message();
	} else
	{
		read_message_.header.author = get_id();
		get_server().process_message(shared_from_this(), read_message_);
		read_message();
	}
}

void user::send_messages_()
{
	message_buffer buffer(messages_queue_.front().header, messages_queue_.front().body);
	boost::asio::async_write(socket_,
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
				Error(get_server().get_logger(), "An error occurred while sending message to user ", get_name(), " (ID: ", get_id(), "): ", ec.message());
				error_quit_();
			}
		});
}

void user::quit_()
{
	try {
		quit_status_ = user_quit;
		socket_.lowest_layer().cancel();
		socket_.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		socket_.shutdown();
		get_server().quit_participant(std::dynamic_pointer_cast<participant>(shared_from_this()), ok);
	} catch (boost::system::system_error &err)
	{
		Error(get_server().get_logger(), "An error occurred while user quitting: ", err.what());
	}
}

void user::error_quit_()
{
	quit_status_ = user_error_quit;
	get_server().quit_participant(std::dynamic_pointer_cast<participant>(shared_from_this()), user_error_quit);
}

