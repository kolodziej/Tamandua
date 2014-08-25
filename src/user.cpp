#include "user.hpp"
#include "message_buffer.hpp"
#include "message_composer.hpp"
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
	bool start_sending = messages_queue_.empty();
	messages_queue_.push_back(message);
	if (start_sending)
		send_messages_();
}

// commands
void user::cmd_id(std::string &params)
{
	message_composer msgc(message_type::info_message);
	msgc << "Your id number is " << get_id() << ".";
	deliver_message(msgc());
}

void user::cmd_msg_id(std::string &params)
{
	message_composer msgc(message_type::info_message);
	msgc << "Last message id is " << get_server().get_last_message_id();
	deliver_message(msgc());
}

void user::cmd_room(std::string &params)
{
	std::stringstream params_stream(params);
	std::string room_name;
	params_stream >> room_name;
	auto room_ptr = get_server().get_group(room_name);
	if (room_ptr == nullptr || room_ptr->is_hidden() == true)
	{
		message_composer msgc(message_type::error_message);
		msgc << "Room called " << room_name << " does not exist!";
		deliver_message(msgc());
	} else
	{
		group_ = room_ptr;
		group_->join_participant(shared_from_this());
		message_composer msgc(message_type::info_message);
		msgc << "Your room is now: " << room_name;
		deliver_message(msgc());
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
		message_composer msgc(message_type::error_message);
		msgc << "Private room called " << room_name << " does not exist!";
		deliver_message(msgc());
	} else
	{
		std::shared_ptr<private_room> proom_ptr = std::dynamic_pointer_cast<private_room>(room_ptr);
		if (proom_ptr->check_password(password))
		{
			group_ = room_ptr;
			room_ptr->join_participant(shared_from_this());
			message_composer msgc(message_type::info_message);
			msgc << "You are now in private room: " << room_name;
			deliver_message(msgc());
		} else
		{
			message_composer msgc(message_type::error_message);
			msgc << "[Private room " << room_name << "]: Wrong password!";
			deliver_message(msgc());
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
		message_composer msgc(message_type::info_message);
		msgc << "You changed your nick from '" << oldname << "' to '" << newname << "'!";
		deliver_message(msgc());
	} else
	{
		message_composer msgc(message_type::error_message);
		msgc << "Nick which you choose (" << newname << ") is in use. Try another one!";
		deliver_message(msgc());
	}
}

void user::cmd_msg(std::string &params)
{
	std::stringstream params_stream(params);
	std::string usr, msg_body;
	params_stream >> usr;
	msg_body = params_stream.str().substr(usr.length() + 1);

	auto rec = get_server().get_participant(usr);
	if (rec != nullptr)
	{
		message msg(message_type::private_message, msg_body);
		msg.header.author = get_id();
		rec->deliver_message(msg);
	} else
	{
		message_composer msgc(message_type::error_message);
		msgc << "There is no user " << usr << "!";
		deliver_message(msgc());
	}
}

void user::cmd_kick_ass(std::string &params)
{
	std::stringstream params_stream(params);
	std::string usr;
	params_stream >> usr;

	auto rec = get_server().get_participant(usr);
	if (rec != nullptr)
	{
		message_composer msgc(message_type::info_message);
		msgc << "User " << get_name() << " kicks your ass!";
		rec->deliver_message(msgc());
	} else
	{
		message_composer msgc(message_type::error_message);
		msgc << "There is no user " << usr << "!";
		deliver_message(msgc());
	}
}

void user::cmd_server_uptime(std::string &params)
{
	message_composer msgc(message_type::info_message);
	msgc << "Server works for: " << get_server().get_uptime_string();
	deliver_message(msgc());
}

void user::cmd_quit(std::string &params)
{
	quit_status_ = user_quit;	
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
			} else if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset)
			{
				error_quit_();
			} else
			{
				TamanduaDebug("Boost error code while reading header: ", ec, " (read length: ", length, ")");
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
				Log(get_server().get_logger(), "User ", get_id(), " passed a message: ", read_message_.body);
				process_message_();
			} else if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset)
			{
				error_quit_();
			} else
			{
				Error(get_server().get_logger(), "User ", get_id(), " failed receiveg a message from client!");
			}
		});
}

void user::process_message_()
{
	get_server().set_message_id(read_message_);
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
			message_composer msgc(message_type::error_message);
			msgc << "Unknown command: [" << read_message_.body << "]! Available commands: " << get_server().get_interpreter().get_commands_list();
			deliver_message(msgc());
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

void user::quit_()
{
	get_server().quit_user(get_id());
}

void user::error_quit_()
{
	get_server().quit_user(get_id(), status::user_error_quit);
}
