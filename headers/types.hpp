#ifndef TYPES_HPP
#define TYPES_HPP

typedef unsigned long long int id_number_t;
typedef enum {
	init_message = 0,
	participants_list = 1,
	rooms_list = 2,
	standard_message = 3,
	error_message = 4,
	info_message = 5
} message_type;

#endif
