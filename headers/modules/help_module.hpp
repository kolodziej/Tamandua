#ifndef TAMANDUA_HELP_MODULE_HPP
#define TAMANDUA_HELP_MODULE_HPP
#include "../module_base.hpp"
#include "../server.hpp"

namespace tamandua
{
	class help_module :
		public module_base
	{
		public:
			help_module(server &, command_interpreter &);

			void cmd_commands(std::shared_ptr<user>, message&);
	};
}

#endif
