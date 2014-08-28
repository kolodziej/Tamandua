#include "types.hpp"
#include "client.hpp"
#include "message.hpp"
#include "message_buffer.hpp"
#include "message_composer.hpp"
#include "logger.hpp"
#include <sstream>
#include <utility>
#include <iostream>

using namespace tamandua;

void client::connect(std::string host, std::string port)
{
	tcp::resolver resolver(io_service_);
	tcp::resolver::iterator endpoint_it = resolver.resolve({ host, port });
	connect(endpoint_it);
}

void client::connect(tcp::resolver::iterator endpoint_iterator)
{
	endpoint_iterator_ = endpoint_iterator;
	boost::asio::async_connect(socket_, endpoint_iterator_,
		[this](boost::system::error_code ec, tcp::resolver::iterator iterator)
		{
			if (!ec)
				call_event_handler_(connecting_succeeded, ok);
			else
				call_event_handler_(connecting_failed, connection_failed);

			read_message_header_();
		});
}

void client::disconnect()
{
	try {
		socket_.shutdown(tcp::socket::shutdown_type::shutdown_both);
		socket_.close();
		call_event_handler_(server_disconnected, ok);
	} catch (boost::system::system_error err)
	{
		call_event_handler_(server_disconnected, server_disconnecting_error);
	}
	connected_ = false;
	call_event_handler_(server_disconnected, ok);
}

id_number_t client::get_id()
{
	return uid_;
}

void client::send_message(message &msg)
{
	msg.header.author = uid_;
	message_buffer buf(msg.header, msg.body);
	boost::asio::async_write(socket_,
		boost::asio::buffer(buf.get_buffer().get(), buf.get_buffer_size()),
		[this](boost::system::error_code ec, size_t length)
		{
			if (!ec)
				call_event_handler_(message_sent, ok);
			else
				call_event_handler_(message_undelivered, ok);
		});
}

bool client::is_next_message()
{
	return (messages_.empty()) ? false : true;
}

std::pair<std::string, message> client::get_next_message()
{
	std::unique_lock<std::mutex> lock(messages_queue_lock_);
	new_message_cv_.wait(lock, std::bind(&client::is_next_message, this));
	message msg = messages_.front();
	messages_.pop_front();
	lock.unlock();

	auto iter = participants_.find(msg.header.author);
	std::string author;
	if (iter != participants_.end())
		author = (*iter).second;

	return make_pair(author, msg);
}

void client::add_event_handler(event_type evt, std::function<void(status)> func)
{
	auto handler = make_pair(evt, func);
	events_handlers_.insert(handler);
}

void client::add_message_()
{
	std::unique_lock<std::mutex> lk(messages_queue_lock_);
	messages_.push_back(std::move(read_message_));
	lk.unlock();

	call_event_handler_(message_received, ok);
	new_message_cv_.notify_one();
}

void client::add_message_(message && msg)
{
	std::unique_lock<std::mutex> lk(messages_queue_lock_);
	messages_.push_back(msg);
	lk.unlock();

	call_event_handler_(message_received, ok);
	new_message_cv_.notify_one();
}

void client::add_message_(message_type type, std::string &body)
{
	#pragma GCC diagnostic warning "-Wformat"
	add_message_(type, std::move(body));
}

void client::add_message_(message_type type, std::string &&body)
{
	#pragma GCC diagnostic warning "-Wformat"
	message msg;
	msg.header.author = 0;
	msg.header.type = type;
	msg.header.size = body.size();
	msg.body = body;

	messages_queue_lock_.lock();
	messages_.push_back(msg);
	messages_queue_lock_.unlock();

	call_event_handler_(message_received, ok);
	new_message_cv_.notify_one();
}

void client::read_message_header_()
{
	boost::asio::async_read(socket_,
		boost::asio::buffer(reinterpret_cast<char*>(&read_message_.header), sizeof(message_header)),
		[this](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				read_message_body_();
			} else if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset)
			{
				TamanduaDebug("Server disconnected!");
				call_event_handler_(server_disconnected, ok);
			}
		});
}

void client::read_message_body_()
{
	std::shared_ptr<char> buffer(new char[read_message_.header.size]);
	boost::asio::async_read(socket_,
		boost::asio::buffer(buffer.get(), read_message_.header.size),
		[this, buffer](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				read_message_.body = std::string(buffer.get(), read_message_.header.size);
				process_message_();
			} else if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset)
			{
				TamanduaDebug("Server disconnected!");
				call_event_handler_(server_disconnected, server_disconnecting_error);
			}
		});
}

void client::process_message_()
{
	message &msg = read_message_;
	if (msg.header.type == message_type::init_message)
	{
		uid_ = msg.header.author;
		call_event_handler_(initialization_data_received, ok);
	} else if (msg.header.type == message_type::participants_list)
	{
		set_participants_list_();
		call_event_handler_(participants_list_received, ok);
	} else if (msg.header.type == message_type::rooms_list)
	{
		set_rooms_list_();
		call_event_handler_(rooms_list_received, ok);
	} else
	{
		add_message_();
	}
	read_message_header_();
}

void client::set_participants_list_()
{
	participants_.clear();
	std::stringstream stream(read_message_.body);
	std::string record, id, name;
	while (stream)
	{
		getline(stream, record, ';');
		size_t colon_pos = record.find_first_of(':');
		if (colon_pos == std::string::npos)
			break;
		id = record.substr(0, colon_pos);
		name = record.substr(colon_pos + 1);
		participants_.insert(make_pair(stoull(id), name));
	}
}

void client::set_rooms_list_()
{
	rooms_.clear();
	std::stringstream stream(read_message_.body);
	std::string record, id, name;
	while (stream)
	{
		getline(stream, record, ';');
		size_t colon_pos = record.find_first_of(':');
		if (colon_pos == std::string::npos)
			break;
		id = record.substr(0, colon_pos);
		name = record.substr(colon_pos + 1);
		rooms_.insert(make_pair(stoull(id), name));
	}
}

void client::call_event_handler_(event_type evt, status st)
{
	auto it = events_handlers_.find(evt);
	if (it == events_handlers_.end())
		return;

	((*it).second)(st);
}
