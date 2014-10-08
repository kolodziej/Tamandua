#ifndef TAMANDUA_MESSAGE_HEADER_HPP
#define TAMANDUA_MESSAGE_HEADER_HPP
#include "types.hpp"
#include <cstdlib>

namespace tamandua
{
	struct message_header
	{
		id_number_t id;
		message_time_t utc_time;
		id_number_t author;
		id_number_t group;
		message_type type;
		message_size size;
		status error_code;
	};
}

#endif
