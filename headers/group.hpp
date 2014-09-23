#ifndef TAMANDUA_GROUP_HPP
#define TAMANDUA_GROUP_HPP
#include "types.hpp"
#include "server.hpp"
#include "participant.hpp"
#include "message.hpp"
#include <string>
#include <memory>
#include <map>

namespace tamandua
{
	class group
	{
		private:
			id_number_t id_;
			std::string name_;
			std::map<id_number_t, std::shared_ptr<participant>> participants_;
			server &server_;
		
		public:
			group(server &svr, std::string name) : id_(svr.get_new_group_id_()), name_(name), server_(svr)
			{
				if (name_.empty())
					name_ = svr.get_default_group_name(id_);
			}

			id_number_t get_id();
			std::string get_name();
			virtual void join_participant(std::shared_ptr<participant>);
			virtual void detach_participant(std::shared_ptr<participant>);
			const std::map<id_number_t, std::shared_ptr<participant>> & get_participants();
			virtual void deliver_message(const message&);
			virtual bool is_hidden() = 0;
			virtual bool is_password() = 0;
			virtual room_type get_type() = 0;

		private:
			void notify_new_participant_(std::shared_ptr<participant>);
	};
}

#endif
