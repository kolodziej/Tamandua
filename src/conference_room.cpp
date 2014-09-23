#include "conference_room.hpp"
#include "exception.hpp"
#include <sstream>

using namespace tamandua;

conference_room::conference_room(server &svr, std::shared_ptr<participant> pt) :
	group(svr, pt->get_name())
{
	if (pt == nullptr)
		throw ptr_nullptr();

	join_participant(pt);
}

conference_room::conference_room(server &svr, std::vector<std::shared_ptr<participant>> &&pts) :
	group(svr, std::string())
{
	if (pts.size() == 0)
		throw std::logic_error("you must add to conference at least one participant!");

	std::stringstream stream;
	const std::string separator(", ");
	for (auto pt : pts)
	{
		if (pt == nullptr)
			throw ptr_nullptr();

		join_participant(pt);
		stream << pt->get_name() << separator;
	}
	std::string name = stream.str();
	set_name(name.substr(0, name.length() - separator.length()));
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
