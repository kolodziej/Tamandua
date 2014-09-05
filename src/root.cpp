#include "root.hpp"
#include "room.hpp"
#include "private_room.hpp"
#include "message_composer.hpp"
#include "utility.hpp"
#include <memory>

using namespace tamandua;

root::root(server &svr, std::string pass) :
	hidden_participant(svr, std::string("root")),
	password_(pass)
{
	if (is_created_ == true)
		throw int(0); // throw proper exception

	is_created_ = true;

	ROOT_CMD("add-room", add_room_);
}

bool root::auth_user(id_number_t id, std::string password)
{
	if (password == password_)
	{
		auth_users_ids_.insert(id);
		Log(get_server().get_logger(), "User with ID: ", id, " gained access to root!");
		return true;
	}

	Log(get_server().get_logger(), "User with ID: ", id, " tried to gain access to root using password: ", password);
	return false;
}

void root::read_message()
{
	message msg = messages_.front();
	messages_.pop_front();
	process_message_(msg);
}

void root::deliver_message(const message &msg)
{
	auto u = auth_users_ids_.find(msg.header.author);
	if (u == auth_users_ids_.end())
	{
		unauthorized_user_(*u);
	} else
	{
		messages_.push_back(msg);
	}
}

void root::process_message_(const message &msg)
{
	std::string msg_body = msg.body;
	auto params = split_params_std(msg_body);
	auto cmd = commands_.find(params[1]);
	if (cmd == commands_.end())
	{
		unknown_command_(msg.header.author, params[1]);
		return;
	}

	(this->*((*cmd).second))(msg);
}

void root::unauthorized_user_(id_number_t id)
{
	auto u = get_server().get_participant(id);
	message_composer msgc(message_type::error_message, "You don't have access to root!");
	u->deliver_message(msgc());
	Log(get_server().get_logger(), "User ", u->get_name(), " (ID: ", u->get_id(), ") tried to obtain access to root!");
}

void root::unknown_command_(id_number_t id, std::string cmd)
{
	auto u = get_server().get_participant(id);
	message_composer msgc(message_type::error_message);
	msgc << "Unknown command: " << cmd;
	u->deliver_message(msgc());
	Log(get_server().get_logger(), "User ", u->get_name(), " (ID: ", u->get_id(), ") tried to call unknown root's command: ", cmd);
}

void root::add_room_(const message &msg)
{
	std::string msg_body = msg.body;
	auto params = split_params_std(msg_body);
	std::shared_ptr<group> new_room = std::make_shared<room>(get_server(), params[2]);
	get_server().add_group(new_room);
	Log(get_server().get_logger(), "Room called `", params[2], "` has been created by user with ID: ", msg.header.author);
}

bool root::is_created_ = false;
