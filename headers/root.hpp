#ifndef TAMANDUA_ROOT_HPP
#define TAMANDUA_ROOT_HPP
#include "hidden_participant.hpp"
#include <string>

namespace tamandua
{
	class root :
		public hidden_participant
	{
		private:
			std::string password_;

		public:
			root(server &svr, std::string pass) : hidden_participant(svr, std::string("root")), password_(pass) {}
			
			void read_message();
			void deliver_message(const message&);
	};
}

#endif
