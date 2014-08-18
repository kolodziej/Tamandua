#ifndef TAMANDUA_ROOM_HPP
#define TAMANDUA_ROOM_HPP
#include "group.hpp"
#include <string>

namespace tamandua
{
	class room :
		public group
	{
		private:
			std::string name_;

		public:
			room(server &svr, std::string & name) : group(svr), name_(std::move(name)) {}
			room(server &svr, std::string && name) : group(svr), name_(name) {}

			virtual bool is_hidden();
	};
}

#endif
