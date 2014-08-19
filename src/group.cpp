#include "group.hpp"
#include "message_header.hpp"

using namespace tamandua;

id_number_t group::get_id()
{
	return id_;
}

std::string group::get_name()
{
	return name_;
}

void group::join_participant(std::shared_ptr<participant> participant)
{
	participants_.push_back(participant);
}

void group::deliver_message(const message &message)
{
	for (auto participant : participants_)
		participant->deliver_message(message);
}
