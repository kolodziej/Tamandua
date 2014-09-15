#include "modules/history_module.hpp"
#include "message_composer.hpp"
#include "user.hpp"
#include "group.hpp"
#include <sstream>

using namespace tamandua;


history_module::history_module(server &svr, command_interpreter &ci, size_t mem_size) :
	module_base(svr, "History module", ci),
	history_size_(mem_size)
{
	MODULE_REGISTER_COMMAND("history", &history_module::cmd_history);
}

history_module::history_user::history_user(server &svr, id_number_t gid, size_t hs) :
	hidden_participant(svr, "history_user_default_name"),
	history_size_(hs)
{
	std::stringstream stream;
	stream << "!history_" << gid;
	set_name(stream.str());
}

void history_module::history_user::deliver_message(const message &msg)
{
	message_type t = msg.header.type;
	if (t == standard_message || t == info_message || t == warning_message)
	{
		history_.push_back(msg);
		if (history_.size() > history_size_)
		{
			while (history_.size() == history_size_)
				history_.pop_front();
		}
		Log(get_server().get_logger(), get_name(), " received new message from ", msg.header.author, " to group ", msg.header.group);
	}
}

const std::deque<message> & history_module::history_user::get_history()
{
	return history_;
}

void history_module::cmd_history(std::shared_ptr<user> u, message &msg)
{
	id_number_t gid = msg.header.group;
	try {
		auto hist = get_history(gid);
		for (auto hmsg : hist)
			u->deliver_message(hmsg);
	} catch (std::logic_error &err)
	{
		Error(get_server().get_logger(), "logic_error: ", err.what());
		message_composer msgc(message_type::error_message, "History module's logic error!", gid);
		u->deliver_message(msgc());
	}
}

void history_module::new_group(std::shared_ptr<group> gr)
{
	auto histuser = std::make_shared<history_user>(get_server(), gr->get_id(), history_size_);
	get_server().add_participant(histuser);
	gr->join_participant(histuser);
	auto ins_p = users_.insert(std::make_pair(gr->get_id(), histuser));
	if (ins_p.second == false)
		Error(get_server().get_logger(), get_name(), " Error: Couldn't add new history user!");
	else
		Log(get_server().get_logger(), get_name(), ": New user for group: ", gr->get_id());
}

const std::deque<message> & history_module::get_history(id_number_t gid)
{
	auto gr_it = users_.find(gid);
	if (gr_it == users_.end())
	{
		Error(get_server().get_logger(), get_name(), " Error: Trying to get history of not existing group with ID: ", gid);
		throw std::logic_error("group not exists!");
	}

	return gr_it->second->get_history();
}
