#ifndef TAMANDUA_GROUP_HPP
#define TAMANDUA_GROUP_HPP
#include "types.hpp"
#include "server.hpp"
#include "participant.hpp"
#include "message.hpp"
#include <string>
#include <memory>
#include <vector>

namespace tamandua
{
	class group
	{
		private:
			id_number_t id_;
			std::string name_;
			std::vector<std::shared_ptr<participant>> participants_;
			server &server_;
		
		public:
			group(server &svr, std::string name) : id_(svr.get_new_group_id_()), name_(name), server_(svr)
			{
				if (name_.empty())
					name_ = svr.get_default_group_name_(id_);
			}

			id_number_t get_id();
			std::string get_name();
			virtual void join_participant(std::shared_ptr<participant>);
			virtual void deliver_message(const message&);
			virtual bool is_hidden() = 0;
			virtual bool is_password() = 0;
	};
}

#endif
