#include "private_room.hpp"

using namespace tamandua;

bool private_room::is_hidden()
{
	return true;
}

bool private_room::is_password()
{
	return true;
}

room_type private_room::get_type()
{
	return gr_private_room;
}

bool private_room::check_password(std::string &pass)
{
	return (pass == password_);
}
