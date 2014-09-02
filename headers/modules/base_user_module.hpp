#ifndef TAMANDUA_BASE_USER_MODULE_HPP
#define TAMANDUA_BASE_USER_MODULE_HPP
#include "module_base.hpp"
#include "server.hpp"

namespace tamandua
{
	class base_user_module :
		public module_base
	{
		public:
			base_user_module(server &, command_interpreter &);

			void preprocessed(std::shared_ptr<user>, message &);
			void postprocessed(std::shared_ptr<user>, message &, processing_status);

			// commands
			void cmd_id(std::shared_ptr<user>, message&);
			void cmd_room(std::shared_ptr<user>, message&);
			void cmd_proom(std::shared_ptr<user>, message&);
			void cmd_nick(std::shared_ptr<user>, message&);
			void cmd_msg(std::shared_ptr<user>, message&);

		private:
			void resp_user_not_exists_(std::shared_ptr<user>, std::string&);
	};
}

#endif
