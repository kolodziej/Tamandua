#include "modules/stat_module.hpp"
#include "user.hpp"
#include "message_composer.hpp"
#include <sstream>
#include <iomanip>

using namespace tamandua;

stat_module::stat_module(server &svr, command_interpreter &ci) :
	module_base(svr, "Statistics module", ci),
	participants_(0),
	groups_(0)
{
	MODULE_REGISTER_COMMAND("statistics", &stat_module::cmd_statistics);
}

void stat_module::message_receive(std::shared_ptr<user> u, message &msg)
{
	++general_.num;
	general_.bytes += msg.body.size();
}

void stat_module::message_processed(std::shared_ptr<user> u, message &msg, processing_status st)
{
	if (st == empty_msg || st == empty_cmd)
		return;

	++processed_.num;
	processed_.bytes += msg.body.size();
	if (st == cmd_processed)
	{
		++commands_.num;
		commands_.bytes += msg.body.size();
	}
}

void stat_module::new_participant(std::shared_ptr<participant> p)
{
	++participants_;
}

void stat_module::new_group(std::shared_ptr<group> g)
{
	++groups_;
}

void stat_module::participant_leave(std::shared_ptr<participant> p)
{
	--participants_;
}

void stat_module::group_close(std::shared_ptr<group> g)
{
	--groups_;
}

void stat_module::cmd_statistics(std::shared_ptr<user> u, message &msg)
{
	message_composer msgc(info_message, msg.header.group);
	msgc << get_name() << "\n"
		<< "All messages: " << general_.num << " (size: " << general_.bytes << " bytes)\n"
		<< "Correctly processed: " << processed_.num << " (size: " << processed_.bytes << " bytes)\n"
		<< "Commands processed: " << commands_.num << " (size: " << commands_.bytes << " bytes)";

	u->deliver_message(msgc());
}
