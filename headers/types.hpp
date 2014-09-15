#ifndef TYPES_HPP
#define TYPES_HPP
#include <chrono>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace tamandua
{
	typedef unsigned char byte;
	typedef unsigned long long int message_time_t;
	typedef std::chrono::system_clock message_time_clock_t;
	typedef unsigned long long int id_number_t;
	typedef unsigned long long int message_size;

	typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket_stream;

	typedef enum {
		init_message = 0,
		participants_list = 1,
		rooms_list = 2,
		standard_message = 3,
		error_message = 4,
		info_message = 5,
		warning_message = 6,
		private_message = 7,
		group_enter_message = 8,
		group_leave_message = 9,
		quit_message = 1000
	} message_type;

	typedef enum {
		bad_cmd = 0,
		cmd_processed = 1,
		std_msg = 2,
		empty_msg = 3,
		empty_cmd = 4
	} processing_status;

	typedef enum {
		ok = 0,
		connection_failed,
		user_quit,
		user_error_quit,
		server_disconnecting_error
	} status;

}

#endif
