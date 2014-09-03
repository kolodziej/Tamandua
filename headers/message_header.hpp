#ifndef TAMANDUA_MESSAGE_HEADER_HPP
#define TAMANDUA_MESSAGE_HEADER_HPP
#include "types.hpp"
#include <cstdlib>

namespace tamandua
{
	struct message_header
	{
		message_time_t utc_time;
		id_number_t author;
		id_number_t group;
		message_type type;
		message_size size;
	};
}

#endif
