#ifndef TAMANDUA_MESSAGE_COMPOSER_HPP
#define TAMANDUA_MESSAGE_COMPOSER_HPP
#include "types.hpp"
#include "message.hpp"
#include <sstream>

namespace tamandua
{
	class message_composer
	{
		private:
			message msg_;
			id_number_t msg_id_, u_id_;
			message_type type_;
			std::stringstream stream_;

		public:
			message_composer(message_type type, id_number_t u_id = 0, id_number_t msg_id = 0) :	msg_id_(msg_id), u_id_(u_id), type_(type)
			{}

			void add() {}
			template <typename First, typename... Rest>
			void add(First f, Rest... r)
			{
				stream_ << f;
				add(r...);
			}

			template <typename Data>
			message_composer & operator<<(Data d)
			{
				stream_ << d;
				return *this;
			}
			
			message & get_msg();

		private:
			void prepare_message_();
			void cleanup_();
	};
}

#endif
