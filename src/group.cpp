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
	auto insertion = participants_.insert(make_pair(p->get_id(), p));
	if (insertion.second)
	{
		message_composer msgc(message_type::group_enter_message, get_name(), get_id());
		p->deliver_message(msgc());
		notify_new_participant_(p);
	}
}

void group::deliver_message(const message &message)
{
	for (auto p : participants_)
		p.second->deliver_message(message);
}

void group::notify_new_participant_(std::shared_ptr<participant> new_p)
{
	message_composer msgc(message_type::info_message, get_id());
	msgc << "User " << new_p->get_name() << " (#" << new_p->get_id() << ") joind to the " << get_name() << "!";
	message msg = msgc();
	for (auto p : participants_)
		p.second->deliver_message(msg);

}
