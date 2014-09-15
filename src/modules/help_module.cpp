#include "modules/help_module.hpp"
#include "user.hpp"
#include "message_composer.hpp"
#include <memory>

using namespace tamandua;

help_module::help_module(server &svr, command_interpreter &ci) :
	module_base(svr, "Help module", ci)
{
	MODULE_REGISTER_COMMAND("commands", &help_module::cmd_commands);
}

void help_module::cmd_commands(std::shared_ptr<user> u, message &msg)
{
	auto commands = get_interpreter().get_all_commands();
	message_composer msgc(message_type::info_message, msg.header.group);
	msgc << "List of commands:\n";
	for (auto cmd : commands)
	{
		if (cmd != "root" && cmd != "root_auth")
			msgc << "\t* " << cmd << "\n";
	}
	u->deliver_message(msgc());
}
