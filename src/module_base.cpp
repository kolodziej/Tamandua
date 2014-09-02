#include "module_base.hpp"

using namespace tamandua;

bool module_base::is_hidden()
{
	return false;
}

command_interpreter& module_base::get_interpreter()
{
	return interpreter_;
}
