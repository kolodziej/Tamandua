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

		message() {}
		message(message_header &p_header, std::string &p_body) : header(std::move(p_header)), body(p_body) {}
		message(message_type type, std::string bd)
		{
			header.type = type;
			header.size = bd.size();
			body = bd;
		}
	};
}

#endif
