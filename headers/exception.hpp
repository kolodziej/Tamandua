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
}

#endif
