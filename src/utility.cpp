#include "utility.hpp"

using namespace tamandua;

std::vector<std::string> split_params_std(std::string &params)
{
	return split_params_base<'\\', ' ', '\"'>(params);
}
