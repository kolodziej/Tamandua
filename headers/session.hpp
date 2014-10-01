#ifndef TAMANDUA_SESSION_HPP
#define TAMANDUA_SESSION_HPP
#include <boost/asio.hpp>

namespace tamandua
{
	typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket_stream;

	class session
	{
		private:
			ssl_socket_stream ssl_stream_;

		public:
			session(boost::asio::io_service&, boost::asio::ssl::context&);
			void start();
			ssl_socket_stream& get_ssl_stream();
			ssl_socket_stream::lowest_layer_type& get_socket();
			std::string get_ip_address();
			void read_message();

		private:
			void perform_handshake_();
			void read_message_header_();
			void read_message_body_();
	};
}

#endif
