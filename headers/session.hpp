#ifndef TAMANDUA_SESSION_HPP
#define TAMANDUA_SESSION_HPP
#include "message.hpp"
#include <functional>
#include <deque>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace tamandua
{
	typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket_stream;
	class server;
	class user;

	class session
	{
		private:
			server &server_;
			ssl_socket_stream ssl_stream_;
			message read_message_;
			std::deque<message> messages_queue_;
			std::shared_ptr<user> user_;

		public:
			session(server&, boost::asio::ssl::context&);
			session(const session&) = delete;
			session(session &&) = delete;
			void start();
			void stop();
			ssl_socket_stream& get_ssl_stream();
			ssl_socket_stream::lowest_layer_type& get_socket();
			std::string get_ip_address();
			void read_message();
			void send_message(message);

			server& get_server();

		private:
			void perform_handshake_();
			void read_message_header_();
			void read_message_body_();
			void send_messages_();
			void process_session_message_();
			void process_message_();
	};
}

#endif
