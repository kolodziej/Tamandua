#ifndef TAMANDUA_SERVER_CONFIG_HPP
#define TAMANDUA_SERVER_CONFIG_HPP
#include <string>

namespace tamandua
{
	struct server_config
	{
		std::string server_name;
		size_t max_participants_number;
		std::string main_room_name;
		std::string root_password;

		std::string participant_name_format;

		server_config() :
			participant_name_format("([0-9a-zA-Z_-]{0,15})")
		{}
	};
}

#endif
