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

void group::detach_participant(std::shared_ptr<participant> p)
{
	auto p_it = participants_.find(p->get_id());
	if (p_it == participants_.end())
	{
		Error(server_.get_logger(), "User with id ", p->get_id(), " does not participate in group ", get_name(), " so he cannot be detached!");
	} else
	{
		message_composer msgc_leave(message_type::group_leave_message, get_id());
		p->deliver_message(msgc_leave());
		participants_.erase(p_it);

		message_composer msgc_info(message_type::info_message, get_id());
		msgc_info << "User " << p->get_name() << " left group!";
		message msg_info = msgc_info();
		deliver_message(msg_info);
	}
}

const std::map<id_number_t, std::shared_ptr<participant>> & group::get_participants()
{
	return participants_;
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
	deliver_message(msg);
}
