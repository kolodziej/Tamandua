#ifndef TAMANDUA_SERVER_CONFIG_HPP
#define TAMANDUA_SERVER_CONFIG_HPP
#include <string>

namespace tamandua
{
	struct server_config
	{
		unsigned int port;
		std::string server_name;
		size_t max_users_number;
		std::string root_password;
		
	}
}

#endif
