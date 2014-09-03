#include "version.hpp"

std::string tamandua::get_version_str()
{
	std::stringstream stream;
	stream << "v" << TAMANDUA_VERSION_MAJOR << "." << TAMANDUA_VERSION_MINOR << "." << TAMANDUA_VERSION_PATCH;
	return stream.str();
}

unsigned short int tamandua::get_version_major()
{
	return TAMANDUA_VERSION_MAJOR;
}

unsigned short int tamandua::get_version_minor()
{
	return TAMANDUA_VERSION_MINOR;
}

unsigned short int tamandua::get_version_patch()
{
	return TAMANDUA_VERSION_PATCH;
}

const char * tamandua::get_release_type()
{
	return TAMANDUA_RELEASE_TYPE;
}
