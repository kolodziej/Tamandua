#ifndef TAMANDUA_COMMAND_INTERPRETER_HPP
#define TAMANDUA_COMMAND_INTERPRETER_HPP
#include "types.hpp"
#include "user.hpp"
#include "message.hpp"
#include <memory>
#include <string>
#include <map>
#include <functional>

namespace tamandua
{
	class command_interpreter
	{
		private:
			std::map<std::string, std::functional<tamandua::processing_status(std::string)>> commands_map_;

		public:	
			void register_command(std::string, std::functional<tamandua::processing_status(std::string)>);
			tamandua::processing_status process(std::shared_ptr<user>, message &);
	}
}

#endif
