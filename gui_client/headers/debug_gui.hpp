#ifndef DEBUG_GUI_HPP
#define DEBUG_GUI_HPP
#ifdef DEBUG
	#include <iostream>
	#include <mutex>
	#define DEBUG_STREAM std::cerr
	#define Debug(...) debug::log(__VA_ARGS__)

class debug
{
	private:
		static std::recursive_mutex write_lock_;
	public:
		static void log()
		{
			DEBUG_STREAM << "\n";
		}

		template <typename First, typename... Rest>
		static void log(First f, Rest... r)
		{
			std::lock_guard<std::recursive_mutex> lock(write_lock_);
			DEBUG_STREAM << f;
			log(r...);
		}
};

#else
	#define Debug(...)
#endif

#endif
