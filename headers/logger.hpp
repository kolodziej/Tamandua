#ifndef TAMANDUA_LOG_HPP
#define TAMANDUA_LOG_HPP
#include <ostream> 

#define LOG_STREAM std::clog
#define ERROR_STREAM std::cerr

#define Log(logger,...) logger.log(__VA_ARGS__)
#define Error(logger,...) logger.error(__VA_ARGS__)

namespace tamandua
{
	class logger
	{
		private:
			std::ostream &stream_;

		public:
			logger(std::ostream &stream) : stream_(stream) {}

			void log()
			{
				stream_ << "\n";
			}
			template <typename F, typename... R>
			void log(F first, R... rest)
			{
				stream_ << first;
				log(rest...);
			}
			void error()
			{
				stream_ << "\n";
			}
			template <typename F, typename... R>
			void error(F first, R... rest)
			{
				stream_ << first;
				error(rest...);
			}
	};
}
#endif
