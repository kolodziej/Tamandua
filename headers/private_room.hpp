#ifndef TAMANDUA_PRIVATE_ROOM_HPP
#define TAMANDUA_PRIVATE_ROOM_HPP
#include "room.hpp"

namespace tamandua
{
	class private_room :
		public room
	{
		public:
			private_room(server &srv, std::string & name) : room(svr, name) {}
			private_room(server &svr, std::string && name) : room(svr, name) {}

			virtual bool is_hidden();

	};
}

#endif
