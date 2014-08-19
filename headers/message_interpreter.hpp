#ifndef TAMANDUA_MESSAGE_INTERPRETER_HPP
#define TAMANDUA_MESSAGE_INTERPRETER_HPP 
#include "types.hpp"
#include "message.hpp"
#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <stdexcept>
#include <iostream>

namespace tamandua
{
	template <typename T, char S>
	class message_interpreter
	{
		private:
			std::map<std::string, void (T::*)(std::string &)> function_map_;

		public:
			message_interpreter() {}

			virtual processing_status process_message(T &u, message &msg)
			{
				try {
					if (msg.body.empty())
						return empty_msg;

					if (msg.body[0] == S)
					{
						if (process_command_(u, msg))
							return cmd_processed;
						else
							return bad_cmd;
					}

					return std_msg;
				} catch (std::out_of_range e)
				{
					return empty_msg;
				}
			}

			std::string get_commands_list()
			{
				std::stringstream stream;
				for (auto f : function_map_)
					stream << f.first << " ";

				return stream.str();
			}

		protected:

			void add_function(std::string key, void (T::*function)(std::string &))
			{
				function_map_.insert(make_pair(key, function));
			}

		private:
			virtual bool process_command_(T &u, message &msg)
			{
				if (msg.body.empty())
					return false;
			
				std::string command, params;
				std::stringstream stream(msg.body);
				stream >> command;
				auto it = function_map_.find(command.substr(1));
				if (it == function_map_.end())
				{
					return false;
				}
				try {
					params = stream.str().substr(command.size() + 1);
				} catch (std::out_of_range e)
				{
					params = std::string();
				}
				(u.*((*it).second))(params);
				return true;
			}

	};
}

#endif
