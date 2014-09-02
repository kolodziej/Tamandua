#include "modules/base_user_module.hpp"
#include "message_composer.hpp"
#include <functional>
#include <sstream>

using namespace tamandua;

base_user_module::base_user_module(server &svr, command_interpreter &interpreter) :
				module_base(svr, "Base User Module", interpreter)
{
	MODULE_REGISTER_COMMAND("id", &base_user_module::cmd_id);
	MODULE_REGISTER_COMMAND("room", &base_user_module::cmd_room);
	MODULE_REGISTER_COMMAND("proom", &base_user_module::cmd_proom);
	MODULE_REGISTER_COMMAND("nick", &base_user_module::cmd_nick);
	MODULE_REGISTER_COMMAND("msg", &base_user_module::cmd_msg);
}

void base_user_module::cmd_id(std::shared_ptr<user> u, message &msg)
{
	message_composer msgc(message_type::info_message);
	auto params = split_params_std(msg.body);
	if (params.size() == 2)
	{
		auto up = get_server().get_participant(params[1]);
		if (user == nullptr)
		{
			resp_user_not_exists_(u, params[1]);
		} else
		{
			msgc << "The id number of user " << up->get_name() << " is " << up->get_id() << ".";
		}
	} else
	{
		msgc << "Your id number is " << u->get_id() << ".";
	}
	u->deliver_message(msgc());
}

void base_user_module::cmd_room(std::shared_ptr<user> u, message &msg)
{
	auto params = split_params_std(msg.body);
	/*auto room_ptr = get_server().get_group(params[1]);
	if (room_ptr == u->get_group())
	{
		message_composer msgc(message_type::warning_message);
		msgc << "You are already subscribed to the room '" << room_name << "'!";
		u->deliver_message(msgc());
	} else if (room_ptr == nullptr || room_ptr->is_hidden() == true)
	{
		message_composer msgc(message_type::error_message);
		msgc << "Room called " << room_name << " does not exist!";
		u->deliver_message(msgc());
	} else
	{
		//u->group_ = room_ptr;
		//group_->join_participant(shared_from_this());
		message_composer msgc(message_type::info_message);
		msgc << "Your room is now: " << room_name;
		u->deliver_message(msgc());
	}*/
}

void base_user_module::cmd_proom(std::shared_ptr<user> u, message &msg)
{
	/*std::stringstream params_stream(params);
	std::string room_name, password;
	params_stream >> room_name >> password;
	auto room_ptr = get_server().get_group(room_name);
	if (room_ptr == nullptr || room_ptr->is_hidden() == false)
	{
		message_composer msgc(message_type::error_message);
		msgc << "Private room called " << room_name << " does not exist!";
		u->deliver_message(msgc());
	} else
	{
		if (room_ptr == group_)
		{
			message_composer msgc(message_type::warning_message);
			msgc << "You are already subscribed to the private room '" << room_name << "'!";
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
				u->deliver_message(msgc());
			} else
			{
				message_composer msgc(message_type::error_message);
				msgc << "[Private room " << room_name << "]: Wrong password!";
				u->deliver_message(msgc());
			}
		}
	}*/
}

void base_user_module::cmd_nick(std::shared_ptr<user> u, message &msg)
{

	std::stringstream params_stream(params);
	std::string newname, oldname = get_name();
	params_stream >> newname;
	if (oldname == newname)
	{
		message_composer msgc(message_type::warning_message);
		msgc << oldname << " == " << oldname << " --- You kidding, right?! :/";
		u->deliver_message(msgc());
	} else if (get_server().change_participant_name(oldname, newname))
	{
		message_composer msgc(message_type::info_message);
		msgc << "You changed your nick from '" << oldname << "' to '" << newname << "'!";
		u->deliver_message(msgc());
	} else
	{
		message_composer msgc(message_type::error_message);
		msgc << "Nick which you choose (" << newname << ") is in use. Try another one!";
		u->deliver_message(msgc());
	}
}

void base_user_module::cmd_msg(std::shared_ptr<user> u, message &msg)
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
		u->deliver_message(msgc());
	}
}

void base_user_module::resp_user_not_exists_(std::shared_ptr<user> u, std::string &uname)
{
	message_composer msgc(message_type::error_message);
	msgc << "User `" << uname << "` doesn't exist!";
	u->deliver_message(msgc());
}
