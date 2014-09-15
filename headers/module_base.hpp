#ifndef TAMANDUA_MODULE_BASE_HPP
#define TAMANDUA_MODULE_BASE_HPP
#include "types.hpp"
#include "message.hpp"
#include "command_interpreter.hpp"
#include "server.hpp"
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
			static id_number_t last_module_id_;
			server &server_;
			id_number_t id_;
			std::string module_name_;
			command_interpreter &interpreter_;

		public:
			module_base(server &svr, std::string name, command_interpreter &interpreter) : server_(svr), id_(++last_module_id_), module_name_(name), interpreter_(interpreter) {}

			id_number_t get_id();
			virtual bool is_hidden();
			virtual void preprocessed(std::shared_ptr<user>, message &) = 0;
			virtual void postprocessed(std::shared_ptr<user>, message &, processing_status) = 0;

			server &get_server();
			command_interpreter &get_interpreter();

	};
}

#endif
