#ifndef TYPES_HPP
#define TYPES_HPP
#include <chrono>

namespace tamandua
{
	typedef unsigned char byte;
	typedef unsigned long long int message_time_t;
	typedef std::chrono::system_clock message_time_clock_t;
	typedef unsigned long long int id_number_t;
	typedef unsigned long long int message_size;

	typedef enum {
		init_message = 0,
		participants_list = 1,
		rooms_list = 2,
		standard_message = 3,
		error_message = 4,
		info_message = 5,
		warning_message = 6,
		private_message = 7
	} message_type;

	typedef enum {
		bad_cmd = 0,
		cmd_processed = 1,
		std_msg = 2,
		empty_msg = 3
	} processing_status;

	typedef enum {
		ok = 0,
		connection_failed,
		user_quit,
		user_error_quit,
		server_disconnecting_error
	} status;

	typedef enum {
		connecting_succeeded = 1,
		connecting_failed,
		message_sent,
		message_undelivered,
		message_received,
		server_disconnected,
		initialization_data_received,
		participants_list_received,
		rooms_list_received,
	} event_type;
}

#endif
