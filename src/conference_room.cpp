#include "conference_room.hpp"
#include "exception.hpp"

using namespace tamandua;

conference_room::conference_room(server &svr, std::string name, std::shared_ptr<participant> pt) :
	group(svr, std::string("@") + name)
{
	if (pt == nullptr)
		throw ptr_nullptr();

	join_participant(pt);
}

conference_room::conference_room(server &svr, std::string name, std::vector<std::shared_ptr<participant>> &&pts) :
	group(svr, std::string("@") + name)
{
	if (pts.size() == 0)
		throw std::logic_error("you must add to conference at least one participant!");

	for (auto pt : pts)
	{
		if (pt == nullptr)
			throw ptr_nullptr();

		join_participant(pt);
	}
}

bool conference_room::is_hidden()
{
	return true;
}

bool conference_room::is_password()
{
	return false;
}

room_type conference_room::get_type()
{
	return gr_conference_room;
}
