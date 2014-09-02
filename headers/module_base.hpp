#ifndef TAMANDUA_MODULE_BASE_HPP
#define TAMANDUA_MODULE_BASE_HPP
#include "types.hpp"
#include "message.hpp"
#include "command_interpreter.hpp"
#include <memory>
#include <string>
#include <functional>

#define MODULE_REGISTER_COMMAND(c, f) get_interpreter().register_command(c, std::bind(f, *this, std::placeholders::_1, std::placeholders::_2))

namespace tamandua
{
	class user;

	class module_base
	{
		private:
			std::string module_name_;
			command_interpreter &interpreter_;

		public:
			module_base(std::string name, command_interpreter &interpreter) : module_name_(name), interpreter_(interpreter) {}

			virtual bool is_hidden();
			virtual void preprocessed(std::shared_ptr<user>, message &) = 0;
			virtual void postprocessed(std::shared_ptr<user>, message &, processing_status) = 0;

			command_interpreter &get_interpreter();
	};
}

#endif
