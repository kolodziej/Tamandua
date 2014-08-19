#ifndef TAMANDUA_USER_MESSAGE_INTERPRETER_HPP
#define TAMANDUA_USER_MESSAGE_INTERPRETER_HPP
#include "message_interpreter.hpp"

namespace tamandua
{
	class user;

	class user_message_interpreter :
		public message_interpreter<user, '/'>
	{
		public:
			user_message_interpreter();
	};
}

#endif
