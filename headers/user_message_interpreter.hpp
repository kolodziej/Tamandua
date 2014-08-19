#ifndef TAMANDUA_USER_MESSAGE_INTERPRETER_HPP
#define TAMANDUA_USER_MESSAGE_INTERPRETER_HPP
#include "message_interperter.hpp"
#include "user.hpp"

namespace tamandua
{
	class user_message_interpreter :
		public message_interpreter<user, '/'>
	{
		private:
			virtual void init_function_map_();
	};
}

#endif
