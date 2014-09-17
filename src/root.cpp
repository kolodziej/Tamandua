#include "root.hpp"
#include "user.hpp"
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
	ROOT_CMD("add-proom", add_private_room_);
	ROOT_CMD("lock-username", lock_username_);
	ROOT_CMD("unlock-username", unlock_username_);
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
{}

void root::deliver_message(const message &msg)
{}

void root::process_command(std::shared_ptr<user> u, std::vector<std::string> &params)
{
	if (auth_users_ids_.find(u->get_id()) == auth_users_ids_.end())
	{
		unauthorized_user_(u);
	} else if (params.empty())
	{
		unknown_command_(u);
	} else
	{
		auto cmd = commands_.find(params[0]);
		if (cmd == commands_.end())
		{
			unknown_command_(u, params[0]);
			return;
		}

		(this->*((*cmd).second))(u, params);
	}
}

std::string root::get_password()
{
	return password_;
}

void root::unauthorized_user_(std::shared_ptr<user> u)
{
	message_composer msgc(message_type::error_message, "You don't have access to root!");
	u->deliver_message(msgc());
	Log(get_server().get_logger(), "User ", u->get_name(), " (ID: ", u->get_id(), ") tried to obtain access to root!");
}

void root::unknown_command_(std::shared_ptr<user> u, std::string cmd)
{
	message_composer msgc(message_type::error_message);
	if (cmd.empty())
		msgc << "You must type root command!";
	else
		msgc << "Unknown command: " << cmd;

	u->deliver_message(msgc());
	Log(get_server().get_logger(), "User ", u->get_name(), " (ID: ", u->get_id(), ") tried to call unknown root's command: ", cmd);
}

void root::add_room_(std::shared_ptr<user> u, std::vector<std::string> &params)
{
	try {
		std::shared_ptr<group> new_room = std::make_shared<room>(get_server(), params[1]);
		get_server().add_group(new_room);
		Log(get_server().get_logger(), "Room called `", params[1], "` has been created by user with ID: ", u->get_id());
		message_composer msgc(message_type::info_message);
		msgc << "You have created room `" << params[1] << "`!";
		u->deliver_message(msgc());
	} catch (std::logic_error &e)
	{
		Error(get_server().get_logger(), "Logic ERROR in root::add_room_: ", e.what());
	} catch (std::runtime_error &e)
	{
		Error(get_server().get_logger(), "Runtime ERROR in root::add_room_: ", e.what());
	}
}

void root::add_private_room_(std::shared_ptr<user> u, std::vector<std::string> &params)
{
	try {
		std::shared_ptr<group> new_proom = std::make_shared<private_room>(get_server(), params[1], params[2]);
		get_server().add_group(new_proom);
		Log(get_server().get_logger(), "Private room called `", params[1], "` has been created by user with ID: ", u->get_id());

		message_composer msgc(message_type::info_message);
		msgc << "You have created private room `" << params[1] << "`!";
		u->deliver_message(msgc());
	} catch (std::logic_error &e)
	{
		Error(get_server().get_logger(), "Logic ERROR in root::add_private_room_: ", e.what());
	} catch (std::runtime_error &e)
	{
		Error(get_server().get_logger(), "Runtime ERROR in root::add_private_room_: ", e.what());
	}
}

void root::lock_username_(std::shared_ptr<user> u, std::vector<std::string> &params)
{
	try {
		if (get_server().lock_username(params[1]))
		{
			message_composer msgc(info_message);
			msgc << "Username " << params[1] << " has been locked!";
			u->deliver_message(msgc());
		} else
		{
			message_composer msgc(warning_message);
			msgc << "Username " << params[1] << " has already been locked!";
			u->deliver_message(msgc());
		}
	} catch (std::runtime_error &e)
	{
		Error(get_server().get_logger(), "Runtime ERROR in root::lock_username_: ", e.what());
	}
}

void root::unlock_username_(std::shared_ptr<user> u, std::vector<std::string> &params)
{
	try {
		if (get_server().unlock_username(params[1]))
		{
			message_composer msgc(info_message);
			msgc << "Username " << params[1] << " has been unlocked!";
			u->deliver_message(msgc());
		} else
		{
			message_composer msgc(warning_message);
			msgc << "Username " << params[1] << " has already been unlocked!";
			u->deliver_message(msgc());
		}
	} catch (std::runtime_error &e)
	{
		Error(get_server().get_logger(), "Runtime ERROR in root::unlock_username_: ", e.what());
	}
}

bool root::is_created_ = false;
