#ifndef TAMANDUA_BASE_USER_MODULE_HPP
#define TAMANDUA_BASE_USER_MODULE_HPP
#include "../module_base.hpp"
#include "../server.hpp"

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
			void cmd_leave(std::shared_ptr<user>, message&);
			void cmd_users(std::shared_ptr<user>, message&);
			void cmd_nick(std::shared_ptr<user>, message&);
			void cmd_server_uptime(std::shared_ptr<user>, message&);
			void cmd_root(std::shared_ptr<user>, message&);
			void cmd_root_auth(std::shared_ptr<user>, message&);

		private:
			void resp_user_not_exists_(std::shared_ptr<user>, std::string&);
			void resp_bad_cmd_format_(std::shared_ptr<user>, std::string);
	};
}

#endif
