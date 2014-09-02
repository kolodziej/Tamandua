#include "modules/base_user_module.hpp"
#include <functional>

base_user_module::base_user_module(command_interpreter &interpreter) :
				module_base("Base User Module", interpreter)
{
	MODULE_REGISTER_COMMAND("id", &base_user_module::cmd_id);
	MODULE_REGISTER_COMMAND("room", &base_user_module::cmd_room);
	MODULE_REGISTER_COMMAND("proom", &base_user_module::cmd_proom);
	MODULE_REGISTER_COMMAND("nick", &base_user_module::cmd_nick);
	MODULE_REGISTER_COMMAND("msg", &base_user_module::cmd_msg);
}
