#include "modules/base_user_module.hpp"
#include "message_composer.hpp"
#include "user.hpp"
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

void base_user_module::preprocessed(std::shared_ptr<user> u, message &msg)
{}

void base_user_module::postprocessed(std::shared_ptr<user> u, message &msg, processing_status s)
{}

void base_user_module::cmd_id(std::shared_ptr<user> u, message &msg)
{
	message_composer msgc(message_type::info_message);
	auto params = split_params_std(msg.body);
	if (params.size() == 2)
	{
		auto up = get_server().get_participant(params[1]);
		if (up == nullptr)
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
	auto room_ptr = get_server().get_group(params[1]);
	if (room_ptr == nullptr || room_ptr->is_hidden() == true)
	{
		message_composer msgc(message_type::error_message);
		msgc << "Room called " << params[1] << " does not exist!";
		u->deliver_message(msgc());
	} else if (u->is_in_group(room_ptr->get_id()))
	{
		message_composer msgc(message_type::warning_message);
		msgc << "You are already subscribed to the room '" << params[1] << "'!";
		u->deliver_message(msgc());
	} else
	{
		room_ptr->join_participant(u);
		u->add_group(room_ptr->get_id());
		message_composer msgc(message_type::group_enter_message, room_ptr->get_id());
		msgc << "Your room is now: " << params[1];
		u->deliver_message(msgc());
	}
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
	auto params = split_params_std(msg.body);
	std::string newname = params[1], oldname = u->get_name();
	if (oldname == newname)
	{
		message_composer msgc(message_type::warning_message);
		msgc << oldname << " == " << oldname << " --- You kidding, right?! :/";
		u->deliver_message(msgc());
	} else if (get_server().change_participant_name(oldname, newname))
	{
		message_composer msgc(message_type::info_message);
		msgc << "User `" << oldname << "` changed nick to `" << newname << "`!";
		get_server().send_to_all(msgc());
	} else
	{
		message_composer msgc(message_type::error_message);
		msgc << "Nick which you choose (" << newname << ") is in use. Try another one!";
		u->deliver_message(msgc());
	}
}

void base_user_module::cmd_msg(std::shared_ptr<user> u, message &msg)
{
	auto params = split_params_std(msg.body);
	std::string pm;
	if (params.size() == 3)
		pm = params[2];
	else
		pm = concat_pieces(params.begin()+2, params.end(), ' ');

	auto rec = get_server().get_participant(params[1]);
	if (rec != nullptr)
	{
		message_composer msgc(message_type::private_message, pm, 0, u->get_id());
		message &pmsg = msgc();
		u->deliver_message(pmsg);
		rec->deliver_message(pmsg);
	} else
	{
		resp_user_not_exists_(u, params[1]);
	}
}

void base_user_module::resp_user_not_exists_(std::shared_ptr<user> u, std::string &uname)
{
	message_composer msgc(message_type::error_message);
	msgc << "User `" << uname << "` doesn't exist!";
	u->deliver_message(msgc());
}
