#ifndef TAMANDUA_CONFERENCE_ROOM_HPP
#define TAMANDUA_CONFERENCE_ROOM_HPP
#include "group.hpp"
#include <vector>

namespace tamandua
{
	class conference_room :
		public group
	{
		public:
			conference_room(server &, std::shared_ptr<participant>);
			conference_room(server &, std::vector<std::shared_ptr<participant>>&&);

			bool is_hidden();
			bool is_password();
			room_type get_type();
	};
}

#endif
