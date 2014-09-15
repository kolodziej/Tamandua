#include "module_base.hpp"

using namespace tamandua;

id_number_t module_base::last_module_id_ = 0;

id_number_t module_base::get_id()
{
	return id_;
}

std::string module_base::get_name()
{
	return module_name_;
}

bool module_base::is_hidden()
{
	return false;
}

server& module_base::get_server()
{
	return server_;
}

command_interpreter& module_base::get_interpreter()
{
	return interpreter_;
}
