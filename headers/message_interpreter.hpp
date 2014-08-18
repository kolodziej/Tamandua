#ifndef TAMANDUA_MESSAGE_INTERPRETER_HPP
#define TAMANDUA_MESSAGE_INTERPRETER_HPP 
#include "types.hpp"
#include "message.hpp"
#include <string>
#include <map>

#define DECLARE_FUNCTION_MAP(x) std::map<std::string, void (x::*)(user &, std::string &)> function_map_;
#define BEGIN_FUNCTION_MAP(x) void x::init_function_map_() {
#define ADD_FUNCTION(x,y) function_map_.insert(std::make_pair(x, y));
#define ADD_FAIL_COMMAND_FUNCTION(y) function_map_.insert(std::make_pair(std::string("_fail_command"), y));
#define END_FUNCTION_MAP() } 

namespace tamandua
{
	class message_interpreter
	{
		private:
			server &server_;
			DECLARE_FUNCTION_MAP(message_interpreter)
		public:
			message_interpreter(server &svr) :
				server_(svr)
			{
				init_function_map_();
			}

			void process_message(user &, message &);
		private:
			void init_function_map_();

	};
}

#endif
