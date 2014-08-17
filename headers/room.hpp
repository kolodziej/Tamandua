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
			room(id_number_t id, std::string & name) : group(id), name_(std::move(name)) {}
			room(id_number_t id, std::string && name) : group(id), name_(name) {}

			virtual bool is_hidden();
	};
}

#endif
