#include "group.hpp"
#include "message_composer.hpp"
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
	notify_new_participant_(p);
}

void group::deliver_message(const message &message)
{
	for (auto p : participants_)
		p.second->deliver_message(message);
}

void group::notify_new_participant_(std::shared_ptr<participant> new_p)
{
	message_composer msgc(message_type::info_message);
	msgc << "User " << new_p->get_name() << " joind to the " << get_name() << "!";
	message msg = msgc();
	for (auto p : participants_)
		p.second->deliver_message(msg);

}
