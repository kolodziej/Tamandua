#include "root.hpp"
#include "utility.hpp"

using namespace tamandua;

root::root(server &svr, std::string pass) :
	hidden_participant(svr, std::string("root")),
	password_(pass)
{
	if (is_created_ == true)
		throw int; // throw proper exception

	is_created_ = true;
}

bool root::auth_user(id_number_t id, std::string password)
{
	if (password == password_)
	{
		auth_users_ids_.insert(id);
		return true;
	}

	return false;
}

void root::read_message()
{
	message msg = messages_.front();
	messages_.pop_front();
	process_message_(msg);
}

void root::deliver_message(const message &msg)
{
	auto u = auth_users_ids_.find(msg.header.author);
	if (u == auth_users_ids_.end())
	{
		unauthorized_user_(*u);
	} else
	{
		messages_.push_back(msg);
	}
}

void root::process_message_(const messsage &msg)
{
	auto params = split_params_std(msg.body);
}

bool root::is_created_ = false;
