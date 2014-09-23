#ifndef TAMANDUA_CONFERENCE_ROOM_HPP
#define TAMANDUA_CONFERENCE_ROOM_HPP
#include "group.hpp"
#include <initializer_list>

namespace tamandua
{
	class conference_room :
		public group
	{
		public:
			conference_room(server &, std::shared_ptr<participant>);
			conference_room(server &, std::initializer_list<std::shared_ptr<participant>>);

			bool is_hidden();
	};
}

#endif
