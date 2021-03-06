#ifndef TAMANDUA_PARTICIPANT_HPP
#define TAMANDUA_PARTICIPANT_HPP
#include "server.hpp"
#include "types.hpp"
#include "message.hpp"
#include <string>
#include <utility>

namespace tamandua
{
	class participant
	{
		private:
			id_number_t id_;
			std::string name_;
			server &server_;
			std::set<id_number_t> groups_;

		public:
			participant(server &svr, std::string & name) : id_(svr.get_new_participant_id_()), name_(std::move(name)), server_(svr)
			{
				if (name_.empty())
					name_ = svr.get_default_user_name(id_);
			};
			participant(server &svr, std::string && name) : id_(svr.get_new_participant_id_()), name_(name), server_(svr)
			{
				if (name_.empty())
					name_ = svr.get_default_user_name(id_);
			};

			id_number_t get_id() const;
			std::string get_name();
			server & get_server();
			void set_name(std::string &);
			void set_name(std::string &&);
			virtual void read_message() = 0;
			virtual void deliver_message(const message&) = 0;
			virtual bool is_hidden();
			bool add_group(id_number_t);
			bool remove_group(id_number_t);
			bool is_in_group(id_number_t);
	};
}

#endif
