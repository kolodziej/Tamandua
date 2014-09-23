#include "types.hpp"
#include "participant.hpp"

using namespace tamandua;

id_number_t participant::get_id() const
{
	return id_;
}

std::string participant::get_name()
{
	return name_;
}

server & participant::get_server()
{
	return server_;
}

void participant::set_name(std::string & name)
{
	name_ = std::move(name);
}

void participant::set_name(std::string && name)
{
	name_ = name;
}

bool participant::is_hidden()
{
	return false;
}

bool participant::add_group(id_number_t id)
{
	auto p = groups_.insert(id);
	return p.second;
}

bool participant::remove_group(id_number_t id)
{
	auto p = groups_.find(id);
	if (p == groups_.end())
		return false;

	groups_.erase(id);
	return true;
}

bool participant::is_in_group(id_number_t id)
{
	auto it = groups_.find(id);
	if (it == groups_.end())
		return false;

	return true;
}

