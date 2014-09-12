#ifndef TAMANDUA_EXCEPTION_HPP
#define TAMANDUA_EXCEPTION_HPP
#include <stdexcept>

namespace tamandua
{
	class user_name_exists :
		public std::logic_error
	{
		explicit user_name_exists() :
			std::logic_error("choosed user name exists!")
		{}
	};

	class group_name_exists :
		public std::logic_error
	{
		explicit group_name_exists() :
			std::logic_error("choosed group name exists!")
		{}
	};
}

#endif
