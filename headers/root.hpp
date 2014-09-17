#ifndef TAMANDUA_ROOT_HPP
#define TAMANDUA_ROOT_HPP
#include "hidden_participant.hpp"
#include "message.hpp"
#include <string>
#include <deque>
#include <set>
#include <vector>
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
			std::map<std::string, void (root::*)(std::shared_ptr<user>, std::vector<std::string>&)> commands_;

		public:
			root(server&, std::string);
			
			bool auth_user(id_number_t, std::string);
			void read_message();
			void deliver_message(const message&);
			void process_command(std::shared_ptr<user>, std::vector<std::string>&);
			std::string get_password();

		private:
			void process_message_(const message);
			void unauthorized_user_(std::shared_ptr<user>);
			void unknown_command_(std::shared_ptr<user>, std::string = std::string());

			// commands
			void add_room_(std::shared_ptr<user>, std::vector<std::string>&);
			void add_private_room_(std::shared_ptr<user>, std::vector<std::string>&);
			void lock_username_(std::shared_ptr<user>, std::vector<std::string>&);
			void unlock_username_(std::shared_ptr<user>, std::vector<std::string>&);
			void reset_username_(std::shared_ptr<user>, std::vector<std::string>&);

	};
}

#endif
