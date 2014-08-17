#ifndef TAMANDUA_MESSAGE_HEADER_HPP
#define TAMANDUA_MESSAGE_HEADER_HPP
#include "types.hpp"
#include <cstdlib>

namespace tamandua
{
	struct message_header
	{
		id_number_t id;
		id_number_t author;
		message_type type;
		size_t size;
	};
}

#endif
