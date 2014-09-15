#include "modules/history_module.hpp"
#include <sstream>

using namespace tamandua;

history_module::history_user::history_user(server &svr, id_number_t gid) :
	hidden_participant(svr, "history_user_default_name")
{
	std::stringstream stream;
	stream << "history_module_user4group_" << gid;
	set_name(stream.str());
}

history_module::history_module(server &svr, command_interpreter &ci) :
	module_base(svr, "History module", ci)
{
}

void history_module::on_server_start()
{
	for (auto gr : get_server().get_all_groups())
		new_group(gr);
}

void history_module::new_group(std::shared_ptr<group> gr)
{
		auto histuser = std::make_shared<history_user>(get_server(), gr->get_id());
		gr->join_participant(histuser);
		users_.push_back(histuser);
}
