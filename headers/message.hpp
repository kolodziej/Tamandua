#ifndef TAMANDUA_MESSAGE_HPP
#define TAMANDUA_MESSAGE_HPP
#include "message_header.hpp"
#include <string>
#include <utility>

namespace tamandua
{
	struct message
	{
		message_header header;
		std::string body;
	};
}

#endif
