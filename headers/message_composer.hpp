#ifndef TAMANDUA_MESSAGE_COMPOSER_HPP
#define TAMANDUA_MESSAGE_COMPOSER_HPP
#include "types.hpp"
#include "message.hpp"
#include "utility.hpp"
#include <sstream>
#include <chrono>

namespace tamandua
{
	class message_composer
	{
		private:
			message msg_;
			id_number_t u_id_;
			message_type type_;
			std::stringstream stream_;

		public:
			message_composer(message_type type, id_number_t u_id = 0) : u_id_(u_id), type_(type)
			{}
			message_composer(message_type type, std::string body, id_number_t u_id = 0) : u_id_(u_id), type_(type), stream_(body)
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
			message & operator()()
			{
				return get_msg();
			}

		private:
			void prepare_message_();
			void cleanup_();
	};
}

#endif
