#ifndef TAMANDUA_BASE_USER_MODULE_HPP
#define TAMANDUA_BASE_USER_MODULE_HPP
#include "module_base.hpp"

namespace tamandua
{
	class base_user_module :
		public module_base
	{
		public:
			base_user_module(command_interpreter &);
	};
}

#endif
