#ifndef TAMANDUA_MODULE_BASE_HPP
#define TAMANDUA_MODULE_BASE_HPP
#include "types.hpp"
#include "user.hpp"
#include "message.hpp"
#include "command_interpreter.hpp"
#include <memory>
#include <string>

namespace tamandua
{
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
	};
}

#endif
