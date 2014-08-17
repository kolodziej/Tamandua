#ifndef TAMANDUA_ROOT_HPP
#define TAMANDUA_ROOT_HPP
#include "hidden_participant.hpp"
#include <string>

namespace tamandua
{
	class root :
		public hidden_participant
	{
		public:
			root() : hidden_participant(0, std::string("root")) {}
			
			void read_message();
			void deliver_message(const message&);
	};
}

#endif
