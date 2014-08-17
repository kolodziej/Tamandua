#ifndef TAMANDUA_PARTICIPANT_HPP
#define TAMANDUA_PARTICIPANT_HPP
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

		public:
			participant(id_number_t id, std::string & name) : id_(id), name_(std::move(name)) {};
			participant(id_number_t id, std::string && name) : id_(id), name_(name) {};

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
