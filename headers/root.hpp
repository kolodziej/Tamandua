#ifndef TAMANDUA_ROOT_HPP
#define TAMANDUA_ROOT_HPP
#include "hidden_participant.hpp"
#include "message.hpp"
#include <string>
#include <deque>
#include <set>
#include <map>

#define ROOT_CMD(x,y) commands_.insert(std::make_pair(x, &root::y))

namespace tamandua
{
	class root :
		public hidden_participant
	{
		private:
			static bool is_created_;
			std::string password_;
			std::set<id_number_t> auth_users_ids_;
			std::deque<message> messages_;
			std::map<std::string, void (root::*)(const message&)> commands_;

		public:
			root(server&, std::string);
			
			bool auth_user(id_number_t, std::string);
			void read_message();
			void deliver_message(const message&);

		private:
			void process_message_(const message &);
			void unauthorized_user_(id_number_t);
			void unknown_command_(id_number_t, std::string);

			// commands
			void add_room_(const message&);

	};
}

#endif
