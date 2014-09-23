#ifndef TAMANDUA_PRIVATE_ROOM_HPP
#define TAMANDUA_PRIVATE_ROOM_HPP
#include "room.hpp"

namespace tamandua
{
	class private_room :
		public room
	{
		private:
			std::string password_;

		public:
			private_room(server &svr, std::string & name, std::string & password) : room(svr, name), password_(password) {}
			private_room(server &svr, std::string && name, std::string && password) : room(svr, name), password_(password) {}

			virtual bool is_hidden();
			virtual bool is_password();
			room_type get_type();

			bool check_password(std::string &);
	};
}

#endif
