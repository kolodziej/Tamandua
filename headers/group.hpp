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
			std::vector<std::shared_ptr<participant>> participants_;
		
		public:
			group(id_number_t id) : id_(id) {}

			id_number_t get_id();
			virtual void join_participant(std::shared_ptr<participant>);
			virtual void deliver_message(const message&);
			virtual bool is_hidden() = 0;
	};
}

#endif
