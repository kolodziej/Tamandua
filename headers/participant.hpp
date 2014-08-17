#ifndef TAMANDUA_PARTICIPANT_HPP
#define TAMANDUA_PARTICIPANT_HPP
#include "server.hpp"
#include "types.hpp"
#include "message.hpp"
#include <string>
#include <memory>
#include <utility>

namespace tamandua
{
	class participant
	{
		private:
			id_number_t id_;
			std::string name_;
			std::shared_ptr<server> server_;

		public:
			participant(std::shared_ptr<server> svr, std::string & name) : id_(svr->get_new_participant_id_()), name_(std::move(name)), server_(svr) {};
			participant(std::shared_ptr<server> svr, std::string && name) : id_(svr->get_new_participant_id_()), name_(name), server_(svr) {};

			id_number_t get_id() const;
			std::string get_name();
			void set_name(std::string &);
			void set_name(std::string &&);
			virtual void read_message() = 0;
			virtual void deliver_message(const message&) = 0;
			virtual bool is_hidden();

	};
}

#endif
