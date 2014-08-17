#ifndef TAMANDUA_HIDDEN_PARTICIPANT_HPP
#define TAMANDUA_HIDDEN_PARTICIPANT_HPP
#include "participant.hpp"

namespace tamandua
{
	class hidden_participant : 
		public participant
	{
		public:
			hidden_participant(id_number_t id, std::string & name) : participant(id, name) {}
			hidden_participant(id_number_t id, std::string && name) : participant(id, name) {}

			virtual void deliver_message(const message&) = 0;
			bool is_hidden();
	};
}

#endif
