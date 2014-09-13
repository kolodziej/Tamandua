#include "modules/base_user_module.hpp"
#include "message_composer.hpp"
#include "user.hpp"
#include "private_room.hpp"
#include "root.hpp"
#include <functional>
#include <sstream>
#include <memory>

using namespace tamandua;

base_user_module::base_user_module(server &svr, command_interpreter &interpreter) :
				module_base(svr, "Base User Module", interpreter)
{
	MODULE_REGISTER_COMMAND("id", &base_user_module::cmd_id);
	MODULE_REGISTER_COMMAND("room", &base_user_module::cmd_room);
	MODULE_REGISTER_COMMAND("proom", &base_user_module::cmd_proom);
	MODULE_REGISTER_COMMAND("leave", &base_user_module::cmd_leave);
	MODULE_REGISTER_COMMAND("nick", &base_user_module::cmd_nick);
	MODULE_REGISTER_COMMAND("msg", &base_user_module::cmd_msg);
	MODULE_REGISTER_COMMAND("server_uptime", &base_user_module::cmd_server_uptime);
	MODULE_REGISTER_COMMAND("root", &base_user_module::cmd_root);
	MODULE_REGISTER_COMMAND("root_auth", &base_user_module::cmd_root_auth);
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
	if (params.size() < 2)
	{
		resp_bad_cmd_format_(u, "/room <proom name>");
	} else if (room_ptr == nullptr || room_ptr->is_hidden() == true)
	{
		message_composer msgc(message_type::error_message);
		msgc << "Room called " << params[1] << " does not exist!";
		u->deliver_message(msgc());
	} else if (u->is_in_group(room_ptr->get_id()))
	{
		message_composer msgc(message_type::warning_message, room_ptr->get_id());
		msgc << "You are already subscribed to the room '" << params[1] << "'!";
		u->deliver_message(msgc());
	} else
	{
		room_ptr->join_participant(u);
		u->add_group(room_ptr->get_id());
	}
}

void base_user_module::cmd_proom(std::shared_ptr<user> u, message &msg)
{
	auto params = split_params_std(msg.body);
	auto room_ptr = get_server().get_group(params[1]);
	if (params.size() < 3)
	{
		resp_bad_cmd_format_(u, "/proom <proom name> <password>");
	} else if (room_ptr == nullptr || room_ptr->is_hidden() == false)
	{
		message_composer msgc(message_type::error_message);
		msgc << "Private room called " << params[1] << " does not exist!";
		u->deliver_message(msgc());
	} else
	{
		if (u->is_in_group(room_ptr->get_id()))
		{
			message_composer msgc(message_type::warning_message);
			msgc << "You are already subscribed to the private room '" << params[1] << "'!";
			u->deliver_message(msgc());
		} else
		{
			std::shared_ptr<private_room> proom_ptr = std::dynamic_pointer_cast<private_room>(room_ptr);
			if (proom_ptr->check_password(params[2]))
			{
				u->add_group(room_ptr->get_id());
				room_ptr->join_participant(u);
				message_composer msgc(message_type::info_message);
				msgc << "You are now in private room: " << params[1];
				u->deliver_message(msgc());
			} else
			{
				message_composer msgc(message_type::error_message);
				msgc << "[Private room " << params[1] << "]: Wrong password!";
				u->deliver_message(msgc());
			}
		}
	}
}

void base_user_module::cmd_leave(std::shared_ptr<user> u, message &msg)
{
	id_number_t uid = u->get_id(), gid = msg.header.group;
	if (u->is_in_group(gid))
	{
		auto gr_ptr = get_server().get_group(gid);
		if (gr_ptr != nullptr)
		{
			gr_ptr->detach_participant(u);
			u->remove_group(gid);
		} else
		{
			Error(get_server().get_logger(), "Leave command: Group with id: ", gid, " does not exist!");
		}
	} else
	{
		Error(get_server().get_logger(), "Leave command: User with id ", uid, " does not participate in group with id ", gid);
	}
}

void base_user_module::cmd_nick(std::shared_ptr<user> u, message &msg)
{
	auto params = split_params_std(msg.body);
	if (params.size() != 2)
	{
		resp_bad_cmd_format_(u, "/nick <new_nick>");
		return;
	}
	std::string newname = params[1], oldname = u->get_name();
	if (oldname == newname)
	{
		message_composer msgc(message_type::warning_message);
		msgc << oldname << " == " << oldname << " --- You kidding, right?! :/";
		u->deliver_message(msgc());
	} else if (get_server().is_username_valid(newname) == false)
	{
		message_composer msgc(message_type::error_message);
		msgc << "Invalid nickname!";
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
	else if (params.size() > 3)
		pm = concat_pieces(params.begin()+2, params.end(), ' ');
	else
	{
		resp_bad_cmd_format_(u, "/msg <recipient_nick> message...");
		return;
	}

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

void base_user_module::cmd_server_uptime(std::shared_ptr<user> u, message &msg)
{
	message_composer msgc(message_type::info_message);
	msgc << "Server is running for: " << get_server().get_uptime_string();
	u->deliver_message(msgc());
}

void base_user_module::cmd_root(std::shared_ptr<user> u, message &msg)
{
	auto params = split_params_std(msg.body);
	std::shared_ptr<root> r = std::dynamic_pointer_cast<root>(get_server().get_participant(0));
	params.erase(params.begin());
	r->process_command(u, params);
}

void base_user_module::cmd_root_auth(std::shared_ptr<user> u, message &msg)
{
	auto params = split_params_std(msg.body);
	if (params.size() != 2)
	{
		resp_bad_cmd_format_(u, "/root_auth <password>");
		return;
	}
	auto r = get_server().get_root();
	if (r->auth_user(u->get_id(), params[1]))
	{
		message_composer msgc(message_type::info_message, "You have been authorized to use root commands!");
		u->deliver_message(msgc());
	} else
	{
		message_composer msgc(message_type::error_message, "Wrong password! Server saved information about your attempt!");
		u->deliver_message(msgc());
	}
}

void base_user_module::resp_user_not_exists_(std::shared_ptr<user> u, std::string &uname)
{
	message_composer msgc(message_type::error_message);
	msgc << "User `" << uname << "` doesn't exist!";
	u->deliver_message(msgc());
}

void base_user_module::resp_bad_cmd_format_(std::shared_ptr<user> u, std::string format)
{
	message_composer msgc(message_type::error_message);
	msgc << "Incorrect command params! Command format: " << format;
	u->deliver_message(msgc());
}
