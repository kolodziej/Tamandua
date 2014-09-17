#ifndef TAMANDUA_REGISTRATION_MODULE_HPP
#define TAMANDUA_REGISTRATION_MODULE_HPP
#include "../module_base.hpp"
#include "../server.hpp"
#include <string>
#include <map>

namespace tamandua
{
	class registration_module :
		public module_base
	{
		private:
			std::map<std::string, std::string> users_;
			
		public:
			registration_module(server &, command_interpreter &);
			registration_module(const registration_module&) = delete;

			void cmd_register(std::shared_ptr<user>, message&);
			void cmd_auth(std::shared_ptr<user>, message&);
			
	};
}

#endif
