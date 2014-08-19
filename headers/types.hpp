#ifndef TYPES_HPP
#define TYPES_HPP

typedef unsigned long long int id_number_t;
typedef unsigned long long int message_size;
typedef enum {
	init_message = 0,
	participants_list = 1,
	rooms_list = 2,
	standard_message = 3,
	error_message = 4,
	info_message = 5,
	quit_message = 1000,
} message_type;
typedef enum {
	bad_cmd = 0,
	cmd_processed = 1,
	std_msg = 2,
	empty_msg = 3
} processing_status;

#endif
