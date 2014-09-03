#ifndef TAMANDUA_VERSION_HPP
#define TAMANDUA_VERSION_HPP
#include "version_config.hpp"
#include <sstream>
#include <string>

namespace tamandua
{
	std::string get_version_str();
	unsigned short int get_version_major();
	unsigned short int get_version_minor();
	unsigned short int get_version_patch();
	const char * get_release_type();
}

#endif
