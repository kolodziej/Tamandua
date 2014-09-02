#ifndef TAMANDUA_MODULE_BASE_HPP
#define TAMANDUA_MODULE_BASE_HPP
#include "types.hpp"
#include <string>

namespace tamandua
{
	class module_base
	{
		private:
			std::string module_name_;

		public:
			module_base(std::string name) : module_name_(name) {}

			virtual bool is_hidden();
	};
}

#endif
