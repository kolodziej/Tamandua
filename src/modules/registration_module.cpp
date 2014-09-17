#include "modules/registration_module.hpp"
#include "utility.hpp"
#include "user.hpp"
#include "message_composer.hpp"

using namespace tamandua;

registration_module::registration_module(server &svr, command_interpreter &ci) :
	module_base(svr, "Registration module", ci)
{
	MODULE_REGISTER_COMMAND("register", &registration_module::cmd_register);
	MODULE_REGISTER_COMMAND("auth", &registration_module::cmd_auth);
}

void registration_module::cmd_register(std::shared_ptr<user> u, message &msg)
{
	auto params = split_params_std(msg.body);
	if (params.size() == 2)
	{
		std::string &password = params[1];
		auto ins_p = users_.insert(std::make_pair(u->get_name(), password));
		if (ins_p.second == true)
		{
			get_server().lock_username(u->get_name());
			message_composer msgc(message_type::info_message, msg.header.group);
			msgc << "Registration completed!";
			u->deliver_message(msgc());
		} else
		{
			message_composer msgc(message_type::error_message, msg.header.group);
			msgc << "Registration failed!";
			u->deliver_message(msgc());
		}
	} else
	{
		message_composer msgc(message_type::error_message, msg.header.group);
		msgc << "Incorrect arguments! Use: " << get_interpreter().get_start_character() << "register <password> to register your current username.";
		u->deliver_message(msgc());
	}
}

void registration_module::cmd_auth(std::shared_ptr<user> u, message &msg)
{
	auto params = split_params_std(msg.body);
	if (params.size() == 3)
	{
		std::string &username = params[1], &password = params[2];
		auto nick = users_.find(username);
		if (nick == users_.end())
		{
			message_composer msgc(error_message, msg.header.group);
			msgc << "Username is not registered!";
			u->deliver_message(msgc());
		} else if (nick->second == password)
		{
			get_server().change_participant_name(u->get_name(), username);
			u->set_name(username);
			message_composer msgc(info_message, msg.header.group);
			msgc << "You have been authorized as " << username << "!";
			u->deliver_message(msgc());
		} else
		{
			message_composer msgc(error_message, msg.header.group);
			msgc << "Wrong password!";
			u->deliver_message(msgc());
		}
	}
}
