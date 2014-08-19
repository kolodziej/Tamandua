#include "user_message_interpreter.hpp"

void user_message_interpreter::init_function_map_()
{
	ADD_FUNCTION("id", User::CmdSendMeId);
	ADD_FUNCTION("quit", User::CmdQuit);
}
