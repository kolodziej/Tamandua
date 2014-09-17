#ifndef TAMANDUA_ROOT_ROOM_HPP
#define TAMANDUA_ROOT_ROOM_HPP
#include "server.hpp"
#include "private_room.hpp"


namespace tamandua
{
	class root_room :
		public private_room
	{
		public:
			root_room(server &);
	};
}
#endif
