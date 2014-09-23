#include "room.hpp"

using namespace tamandua;

bool room::is_hidden()
{
	return false;
}

bool room::is_password()
{
	return false;
}

room_type room::get_type()
{
	return gr_room;
}
