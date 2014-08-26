#include "group.hpp"
#include "message_header.hpp"
#include <utility>

using namespace tamandua;

id_number_t group::get_id()
{
	return id_;
}

std::string group::get_name()
{
	return name_;
}

void group::join_participant(std::shared_ptr<participant> p)
{
	participants_.insert(make_pair(p->get_id(), p));
}

void group::deliver_message(const message &message)
{
	for (auto p : participants_)
		p.second->deliver_message(message);
}
