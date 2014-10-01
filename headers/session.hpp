#ifndef TAMANDUA_SESSION_HPP
#define TAMANDUA_SESSION_HPP
#include "message.hpp"
#include <functional>
#include <boost/asio.hpp>

namespace tamandua
{
	typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket_stream;
	class server;

	class session
	{
		private:
			server &server_;
			ssl_socket_stream ssl_stream_;
			message read_message_;
			std::function<void(message&)> message_received_callback_;

		public:
			session(server&, boost::asio::ssl::context&);
			session(session &&);
			void start(std::function<void()>, std::function<void(message&&)>);
			void stop();
			ssl_socket_stream& get_ssl_stream();
			ssl_socket_stream::lowest_layer_type& get_socket();
			std::string get_ip_address();
			void read_message();

			server& get_server();

		private:
			void perform_handshake_(std::function<void()>);
			void read_message_header_();
			void read_message_body_();
	};
}

#endif
