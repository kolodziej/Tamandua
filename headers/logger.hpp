#ifndef TAMANDUA_LOG_HPP
#define TAMANDUA_LOG_HPP
#include <iostream> 
#include <iomanip>
#include <chrono>
#include <mutex>

#define LOG_STREAM std::clog
#define ERROR_STREAM std::cerr

#define Log(logger,...) logger.log("[", std::setprecision(6), std::setw(12), std::fixed, logger.get_time(), "] ", __VA_ARGS__)
#define Error(logger,...) logger.error("[", std::setprecision(6), std::setw(12), std::fixed, logger.get_time(), "] ", __VA_ARGS__)
#ifdef DEBUG
	#define TamanduaDebug(...) logger::debug(__VA_ARGS__)
#else
	#define TamanduaDebug(...)
#endif

namespace tamandua
{
	class logger
	{
		private:
			std::ostream &stream_;
			std::chrono::high_resolution_clock::time_point start_time_;
			std::recursive_mutex write_lock_;

		public:
			logger(std::ostream &stream) : stream_(stream), start_time_(std::chrono::high_resolution_clock::now()) {}

			double get_time()
			{
				return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - start_time_).count();
			}

			void log()
			{
				std::lock_guard<std::recursive_mutex> lock(write_lock_);
				stream_ << "\n";
			}
			template <typename F, typename... R>
			void log(F first, R... rest)
			{
				std::lock_guard<std::recursive_mutex> lock(write_lock_);
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
#ifdef DEBUG
			static void debug()
			{
				std::cerr << "\n";
			}

			template <typename F, typename... R>
			static void debug(F f, R... r)
			{
				std::cerr << f;
				debug(r...);
			}
#endif
	};
}
#endif
