#ifndef TAMANDUA_EXCEPTION_HPP
#define TAMANDUA_EXCEPTION_HPP
#include <stdexcept>

namespace tamandua
{
	struct user_name_exists :
		std::logic_error
	{
		explicit user_name_exists() :
			std::logic_error("choosed user name exists!")
		{}
	};

	struct group_name_exists :
		std::logic_error
	{
		explicit group_name_exists() :
			std::logic_error("choosed group name exists!")
		{}
	};

	struct module_already_registered :
		std::logic_error
	{
		explicit module_already_registered() :
			std::logic_error("module has already been registered")
		{}
	};
}

#endif
