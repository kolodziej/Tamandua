#ifndef TAMANDUA_USER_HPP
#define TAMANDUA_USER_HPP
#include "group.hpp"
#include "participant.hpp"
#include "message.hpp"
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
	typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket_stream;

	class user :
		public participant,
		public std::enable_shared_from_this<user>
	{
		private:
			ssl_socket_stream socket_;
			std::deque<message> messages_queue_;
			message read_message_;
			status quit_status_;
			std::chrono::time_point<message_time_clock_t> locked_until_;
			
		public:
			friend class server;
			user(server &svr, std::string & name, boost::asio::ssl::context &context) : participant(svr, name), socket_(svr.get_io_service(), context), quit_status_(ok)
			{}
			user(server &svr, std::string && name, boost::asio::ssl::context &context) : participant(svr, name), socket_(svr.get_io_service(), context), quit_status_(ok)
			{}
			~user();

			void start();
			ssl_socket_stream::lowest_layer_type &get_socket();
			std::string get_ip_address();
			
			virtual void read_message();
			virtual void deliver_message(const message&);
			void lock(unsigned int, std::string = std::string());
			void unlock(std::string = std::string());
			bool is_locked();
			void remove(std::string);
			void quit();
			
		private:
			void perform_handshake_();
			void add_to_hall_();
			void read_message_header_();
			void read_message_body_();
			void process_message_();
			void process_init_message_();
			void send_messages_();

			void quit_();
			void error_quit_();
	};
}

#endif
