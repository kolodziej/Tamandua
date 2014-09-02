#ifndef TAMANDUA_BASE_USER_MODULE_HPP
#define TAMANDUA_BASE_USER_MODULE_HPP
#include "module_base.hpp"

namespace tamandua
{
	class base_user_module :
		public module_base
	{
		public:
			base_user_module(server &, command_interpreter &);

			// commands
			void cmd_id(std::shared_ptr<user>, message&);
			void cmd_room(std::shared_ptr<user>, message&);
			void cmd_proom(std::shared_ptr<user>, message&);
			void cmd_nick(std::shared_ptr<user>, message&);
			void cmd_msg(std::shared_ptr<user>, message&);
	};
}

#endif
