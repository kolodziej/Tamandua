#include "root_room.hpp"

using namespace tamandua;

root_room::root_room(server &svr) :
	private_room(svr, "_root_room", svr.get_root()->get_password())
{}

void root_room::deliver_message(const message& msg)
{
	
}
