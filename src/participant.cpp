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
