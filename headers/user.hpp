#ifndef TAMANDUA_USER_HPP
#define TAMANDUA_USER_HPP
#include "group.hpp"
#include "participant.hpp"
#include "message.hpp"
#include "session.hpp"
#include <string>
#include <deque>
#include <memory>
#include <set>
#include <utility>
#include <chrono>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace tamandua
{
	class user :
		public participant,
		public std::enable_shared_from_this<user>
	{
		private:
			session &session_;
			std::deque<message> messages_queue_;
			message read_message_;
			status quit_status_;
			std::chrono::time_point<message_time_clock_t> locked_until_;
			
		public:
			friend class server;
			user(server &svr, session& sess, std::string & name) : participant(svr, name), session_(sess), quit_status_(ok)
			{}
			user(server &svr, session& sess, std::string && name) : participant(svr, name), session_(sess), quit_status_(ok)
			{}
			~user();

			session& get_session();
			virtual void read_message();
			virtual void deliver_message(const message&);
			void lock(unsigned int, std::string = std::string());
			void unlock(std::string = std::string());
			bool is_locked();
			void remove(std::string);
			void quit();
			
		private:
			void add_to_hall_();

			void quit_();
			void error_quit_();
	};
}

#endif
