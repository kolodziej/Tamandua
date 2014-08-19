#include "user_message_interpreter.hpp"
#include "user.hpp"

using namespace tamandua;

user_message_interpreter::user_message_interpreter()
{
	add_function("id", &user::cmd_id);
	add_function("room", &user::cmd_room);
	add_function("proom", &user::cmd_proom);
	add_function("nick", &user::cmd_nick);
	add_function("msg", &user::cmd_msg);
	add_function("quit", &user::cmd_quit);
}
