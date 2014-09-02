#include "module_base.hpp"

using namespace tamandua;

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
