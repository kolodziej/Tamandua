#include "types.hpp"
#include "client.hpp"
#include "message.hpp"
#include "message_buffer.hpp"
#include <sstream>
#include <utility>
#include <iostream>

using namespace tamandua;

void client::connect()
{
	boost::asio::async_connect(socket_, endpoint_iterator_,
		[this](boost::system::error_code ec, tcp::resolver::iterator iterator)
		{
			if (!ec)
			{
				add_message_(message_type::info_message, std::string("Connection initialized! Waiting for init data from server..."));
				read_message_header_();
			} else
			{
				add_message_(message_type::error_message, std::string("Establishing connection failed!"));
			}
		});
}

id_number_t client::get_id()
{
	return uid_;
}

void client::send_message(message &msg)
{
	msg.header.author = uid_;
	msg.header.id = 0;
	msg.header.type = message_type::standard_message;
	message_buffer buf(msg.header, msg.body);

	boost::asio::async_write(socket_,
		boost::asio::buffer(buf.get_buffer().get(), buf.get_buffer_size()),
		[this](boost::system::error_code ec, size_t length)
		{
			if (ec)
			{
				add_message_(message_type::error_message, std::string("Error while sending message!"));
			}
		});
}

bool client::is_next_message()
{
	std::lock_guard<std::mutex> lock(messages_queue_lock_);
	return (messages_.empty()) ? false : true;
}

std::pair<std::string, message> client::get_next_message()
{
	message msg = messages_.front();
	messages_queue_lock_.lock();
	messages_.pop_front();
	messages_queue_lock_.unlock();
	auto iter = participants_.find(msg.header.author);
	std::string author;
	if (iter != participants_.end())
		author = (*iter).second;

	return make_pair(author, msg);
}

void client::add_message_()
{
	std::lock_guard<std::mutex> lock(messages_queue_lock_);
	messages_.push_back(std::move(read_message_));
}

void client::add_message_(message_type type, std::string &body)
{
	add_message_(type, std::move(body));
}

void client::add_message_(message_type type, std::string &&body)
{
	message msg;
	msg.header.id = 0;
	msg.header.author = 0;
	msg.header.type = type;
	msg.header.size = body.size();
	msg.body = body;

	messages_queue_lock_.lock();
	messages_.push_back(msg);
	messages_queue_lock_.unlock();
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
			}
		});
}

void client::process_message_()
{
	message &msg = read_message_;
	if (msg.header.type == message_type::init_message)
	{
		uid_ = msg.header.author;
		std::stringstream stream;
		stream << "Initialization data received! Your user id number is " << uid_ << ". Your nickname is " << msg.body << ". You can change it using /nick <new_nick> command!";
		add_message_(message_type::info_message, stream.str());
	} else if (msg.header.type == message_type::participants_list)
	{
		set_participants_list_();
	} else if (msg.header.type == message_type::rooms_list)
	{
		set_rooms_list_();
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
