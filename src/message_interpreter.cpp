#include "message_interpreter.hpp"
#include "user.hpp"
#include <sstream>

void message_interpreter::process_message(user &, message &msg)
{
	std::string &bd = msg.body;
	if (bd[0] == '/')
	{
		process_command_(msg);
	} else
	{
		process_not_command_(msg);
	}
}

void message::interpreter::process_command_(message &msg)
{
	std::string command, params;
	std::stringstream stream(msg.body.substr(1));
	stream >> command;
	auto it = function_map_.find(command);
	if (it == function_map_.end())
	{
		auto fail = function_map_.find("_fail_command");
		if (fail == function_map_.end())
			return;
		else
			(*fail)(msg);
	}
	params = stream.str().substr(command.size() + 1);
	// call command function
}
