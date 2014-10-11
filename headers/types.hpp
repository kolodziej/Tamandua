#ifndef TYPES_HPP
#define TYPES_HPP
#include <chrono>
#include <cstdint>

namespace tamandua
{
	typedef unsigned char byte;
	typedef uint64_t message_time_t;
	typedef std::chrono::system_clock message_time_clock_t;
	typedef uint64_t id_number_t;
	typedef uint64_t message_size;

	typedef enum : uint64_t {
		auth_message = 0,
		init_message,
		participants_list,
		rooms_list,
		standard_message,
		error_message,
		info_message,
		warning_message,
		group_enter_message,
		group_leave_message,
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

	typedef enum {
		gr_room = 0,
		gr_private_room,
		gr_conference_room
	} room_type;
}

#endif
