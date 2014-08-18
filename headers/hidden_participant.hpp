#ifndef TAMANDUA_HIDDEN_PARTICIPANT_HPP
#define TAMANDUA_HIDDEN_PARTICIPANT_HPP
#include "participant.hpp"

namespace tamandua
{
	class hidden_participant : 
		public participant
	{
		public:
			hidden_participant(server &svr, std::string & name) : participant(svr, name) {}
			hidden_participant(server &svr, std::string && name) : participant(svr, name) {}

			virtual void deliver_message(const message&) = 0;
			bool is_hidden();
	};
}

#endif
