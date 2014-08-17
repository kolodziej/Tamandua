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
			root(std::shared_ptr<server> svr) : hidden_participant(svr, std::string("root")) {}
			
			void read_message();
			void deliver_message(const message&);
	};
}

#endif
