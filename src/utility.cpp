#include "utility.hpp"

using namespace tamandua;

std::vector<std::string> tamandua::split_params_std(std::string &params)
{
	return split_params_base<'\\', ' ', '\"'>(params);
}

std::string tamandua::concat_pieces(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, char glue)
{
	std::string s;
	for (auto it = begin; it != end; ++it)
	{
		s += (*it);
		if (it + 1 != end)
			s.push_back(glue);
	}

	return s;
}
