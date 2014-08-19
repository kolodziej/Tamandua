#ifndef TAMANDUA_MESSAGE_INTERPRETER_HPP
#define TAMANDUA_MESSAGE_INTERPRETER_HPP 
#include "types.hpp"
#include "message.hpp"
#include <string>
#include <sstream>
#include <map>

#define ADD_FUNCTION(x,y) function_map_.insert(std::make_pair(x, y))

namespace tamandua
{
	template <typename T, char S>
	class message_interpreter
	{
		private:
			std::map<std::string, void (T::*)(std::string &)> function_map_;

		public:
			enum processing_status {
				bad_cmd = 0,
				cmd_processed = 1,
				std_msg = 2
			};

			message_interpreter()
			{
				init_function_map_();
			}

			virtual processing_status process_message(T &u, message &msg)
			{
				std::string &bd = msg.body;
				if (msg.body[0] != S)
				{
					if (process_command_(u, msg))
						return cmd_processed;
					else
						return bad_cmd;
				}

				return std_msg;
			}

		private:
			virtual bool process_command_(T &u, message &msg)
			{
				std::string command, params;
				std::stringstream stream(msg.body.substr(1));
				stream >> command;
				auto it = function_map_.find(command);
				if (it == function_map_.end())
				{
					return false;
				}
				params = stream.str().substr(command.size() + 1);
				(u.*it)(params);

				return true;
			}
			virtual void init_function_map_() = 0;

	};
}

#endif
