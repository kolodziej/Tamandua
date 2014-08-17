#ifndef TAMANDUA_LOG_HPP
#define TAMANDUA_LOG_HPP
#include <iostream> 

#define LOG_STREAM std::clog
#define ERROR_STREAM std::cerr

#ifdef DEBUG
	#define Log(...) tamandua::log(__VA_ARGS__)
	#define Error(...) tamandua::error(__VA_ARGS__)
#else
	#define Log(...)
	#define Error(...)
#endif

namespace tamandua
{
	template <typename T, typename... Args>
	void stream_log(std::ostream & stream, T arg, Args... args)
	{
		stream << arg;
		stream_log(args...);
		stream << std::endl;
	}

	template <typename T, typename... Args>
	void log(T arg, Args... args)
	{
		LOG_STREAM << arg;
		log(args...);
		LOG_STREAM << std::endl;
	}

	template <typename T, typename... Args>
	void error(T arg, Args... args)
	{
		ERROR_STREAM << arg;
		error(args...);
		ERROR_STREAM << std::endl;
	}

}

#endif
