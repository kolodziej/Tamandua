#ifndef DEBUG_GUI_HPP
#define DEBUG_GUI_HPP
#ifdef DEBUG
	#include <iostream>
	#define DEBUG_STREAM std::cerr
	#define Debug(...) debug::log(__VA_ARGS__)
#else
	#define Debug(...)
#endif

#ifdef DEBUG
class debug
{
	public:
		static void log()
		{
			DEBUG_STREAM << "\n";
		}

		template <typename First, typename... Rest>
		static void log(First f, Rest... r)
		{
			DEBUG_STREAM << f;
			log(r...);
		}
};
#endif

#endif
