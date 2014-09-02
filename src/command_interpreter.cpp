#include "command_interpreter.hpp"

using namespace tamandua;

void command_interpreter::register_command(std::string cmd, std::function<void(std::shared_ptr<user>, message&)> func)
{
	auto add_info = commands_map_.insert(make_pair(cmd, func));
	if (add_info.second == false)
	{
		// throw an exception
	}
}

processing_status command_interpreter::process(std::shared_ptr<user> u, message &msg)
{
	if (msg.body.empty())
		return empty_msg;

	if (msg.body[0] == start_character_)
	{
		 return process_command_(u, msg);
	}
	
	return processing_status::std_msg;
}

processing_status command_interpreter::process_command_(std::shared_ptr<user> u, message &msg)
{
	std::stringstream stream(msg.body);
	std::string command;
	stream >> command;
	if (command.empty())
		return empty_cmd;

	auto cmd_it = commands_map_.find(command);
	if (cmd_it == commands_map_.end())
		return bad_cmd;
	else
	{
		((*cmd_it).second)(u, msg);
		return cmd_processed;
	}
}
