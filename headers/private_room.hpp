#ifndef TAMANDUA_PRIVATE_ROOM_HPP
#define TAMANDUA_PRIVATE_ROOM_HPP
#include "room.hpp"

namespace tamandua
{
	class private_room :
		public room
	{
		public:
			private_room(id_number_t id, std::string & name) : room(id, name) {}
			private_room(id_number_t id, std::string && name) : room(id, name) {}

			virtual bool is_hidden();

	};
}

#endif
