#include "user.hpp"
#include "message_buffer.hpp"
#include "room.hpp"
#include "private_room.hpp"
#include <utility>
#include <iostream>

using namespace tamandua;

user::~user()
{
	socket_.cancel();
	socket_.close();
	Log(get_server().get_logger(), "User ID ", get_id(), " is leaving server!");
}

std::string user::get_ip_address()
{
	return socket_.remote_endpoint().address().to_string();
}

void user::read_message()
{
	if (quit_)
	{
		get_server().quit_user(get_id());
		return;
	}
	
	read_message_header_();
}

void user::deliver_message(const message &message)
{
	bool start_sending = messages_queue_.empty();
	messages_queue_.push_back(message);
	if (start_sending)
		send_messages_();
}

// commands
void user::cmd_id(std::string &params)
{
	std::stringstream str;
	str << "Your id number is " << get_id();
	message msg(message_type::info_message, str.str());
	deliver_message(msg);
}

void user::cmd_room(std::string &params)
{
	std::stringstream params_stream(params);
	std::string room_name;
	params_stream >> room_name;
	auto room_ptr = get_server().get_group(room_name);
	if (room_ptr == nullptr || room_ptr->is_hidden() == true)
	{
		std::stringstream stream;
		stream << "Room called " << room_name << " does not exist!";
		message msg(message_type::error_message, stream.str());
		deliver_message(msg);
	} else
	{
		group_ = room_ptr;
		group_->join_participant(shared_from_this());
		std::stringstream stream;
		stream << "Your room is now: " << room_name;
		message msg(message_type::info_message, stream.str());
		deliver_message(msg);
	}
}

void user::cmd_proom(std::string &params)
{
	std::stringstream params_stream(params);
	std::string room_name, password;
	params_stream >> room_name >> password;
	auto room_ptr = get_server().get_group(room_name);
	if (room_ptr == nullptr || room_ptr->is_hidden() == false)
	{
		std::stringstream stream;
		stream << "Private room called " << room_name << " does not exist!";
		message msg(message_type::error_message, stream.str());
		deliver_message(msg);
	} else
	{
		std::shared_ptr<private_room> proom_ptr = std::dynamic_pointer_cast<private_room>(room_ptr);
		if (proom_ptr->check_password(password))
		{
			group_ = room_ptr;
			room_ptr->join_participant(shared_from_this());
			std::stringstream stream;
			stream << "You are now in private room: " << room_name;
			message msg(message_type::info_message, stream.str());
			deliver_message(msg);
		} else
		{
			message msg(message_type::error_message, "Wrond password!");
			deliver_message(msg);
		}
	}
}

void user::cmd_nick(std::string &params)
{
	std::stringstream params_stream(params);
	std::string newname, oldname = get_name();
	params_stream >> newname;
	if (get_server().change_participant_name(oldname, newname))
	{
		std::stringstream stream;
		stream << "You changed your nick from '" << oldname << "' to '" << newname << "!";
		message msg(message_type::info_message, stream.str());
		deliver_message(msg);
	} else
	{
		std::stringstream stream;
		stream << "Nick which you choose (" << newname << ") is in use. Try another one!";
		message msg(message_type::error_message, stream.str());
		deliver_message(msg);
	}
}

void user::cmd_msg(std::string &params)
{

}

void user::cmd_quit(std::string &params)
{
	quit_ = true;	
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
				Log(get_server().get_logger(), "User ", get_id(), " passed a message!");
				process_message_();
			} else
			{
				Error(get_server().get_logger(), "User ", get_id(), " failed receiveg a message from client!");
			}
		});
}

void user::process_message_()
{
	switch (get_server().get_interpreter().process_message(*this, read_message_))
	{
		case processing_status::std_msg:
			if (group_)
				group_->deliver_message(read_message_);
			else
			{
				message msg(message_type::error_message, std::string("You must select group using /room <name> or /proom <name> <password>. Your client received full list of public rooms."));
				deliver_message(msg);
			}
			break;

		case processing_status::bad_cmd:
			std::stringstream str;
			str << "Unknown command: [" << read_message_.body << "]! Available commands: " << get_server().get_interpreter().get_commands_list();
			message msg(message_type::error_message, str.str());
			deliver_message(msg);
			break;
	}
	read_message();
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
