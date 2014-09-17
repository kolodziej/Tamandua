#ifndef TAMANDUA_COMMAND_INTERPRETER_HPP
#define TAMANDUA_COMMAND_INTERPRETER_HPP
#include "types.hpp"
#include "message.hpp"
#include <memory>
#include <string>
#include <map>
#include <functional>

namespace tamandua
{
	class user;

	class command_interpreter
	{
		private:
			const char start_character_;
			std::map<std::string, std::function<void(std::shared_ptr<user>, message&)>> commands_map_;

		public:
			command_interpreter(char start_character) : start_character_(start_character) {}
			void register_command(std::string, std::function<void(std::shared_ptr<user>, message&)>);
			std::vector<std::string> get_all_commands();
			processing_status process(std::shared_ptr<user>, message &);
			char get_start_character();

		private:
			processing_status process_command_(std::shared_ptr<user>, message &);
	};
}

#endif
