#ifndef TAMANDUA_UTILITY_HPP
#define TAMANDUA_UTILITY_HPP
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <memory>

namespace tamandua
{
	template <typename Clock, int BufSize>
	std::string format_time(std::chrono::time_point<Clock> timepoint, std::string format, tm * (*func)(const time_t*))
	{
		char buffer[BufSize];
		time_t timepoint_t = Clock::to_time_t(timepoint);
		tm * tm_struct = (*func)(&timepoint_t);
		strftime(buffer, BufSize, format.data(), tm_struct);
		return std::string(buffer);
	}

	template <typename Clock, int BufSize>
	std::string format_localtime(std::chrono::time_point<Clock> timepoint, std::string format)
	{
		return format_time<Clock, BufSize>(timepoint, format, &localtime);
	}

	template <typename Clock, int BufSize>
	std::string format_utc_time(std::chrono::time_point<Clock> timepoint, std::string format)
	{
		return format_time<Clock, BufSize>(timepoint, format, &gmtime);
	}

}

#endif
