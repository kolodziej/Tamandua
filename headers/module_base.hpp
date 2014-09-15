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

			virtual void on_server_start() {};
			virtual void message_receive(std::shared_ptr<user>, message &) {};
			virtual void message_processed(std::shared_ptr<user>, message &, processing_status) {};
			virtual void new_participant(std::shared_ptr<participant>) {};
			virtual void new_group(std::shared_ptr<group>) {};
			virtual void participant_leave(std::shared_ptr<participant>) {};
			virtual void group_close(std::shared_ptr<group>) {};

			server &get_server();
			command_interpreter &get_interpreter();

	};
}

#endif
