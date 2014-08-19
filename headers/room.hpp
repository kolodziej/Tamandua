#ifndef TAMANDUA_ROOM_HPP
#define TAMANDUA_ROOM_HPP
#include "group.hpp"
#include <string>

namespace tamandua
{
	class room :
		public group
	{
		public:
			room(server &svr, std::string & name) : group(svr, name) {}
			room(server &svr, std::string && name) : group(svr, name) {}

			virtual bool is_hidden();
	};
}

#endif
