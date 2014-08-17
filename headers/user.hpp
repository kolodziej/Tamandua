#ifndef TAMANDUA_USER_HPP
#define TAMANDUA_USER_HPP
#include "group.hpp"
#include "participant.hpp"
#include "message.hpp"
#include <string>
#include <deque>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace tamandua
{
	class user :
		public participant
	{
		private:
			tcp::socket socket_;
			std::deque<message> messages_queue_;
			std::shared_ptr<group> group_;
			message read_message_;
		public:
			user(id_number_t id, std::string & name, tcp::socket socket) : participant(id, name), socket_(std::move(socket)) {}
			user(id_number_t id, std::string && name, tcp::socket socket) : participant(id, name), socket_(std::move(socket)) {}
			
			virtual void read_message();
			virtual void deliver_message(const message&);

		private:
			void read_message_header_();
			void read_message_body_();
			void send_messages_();

	};
}

#endif
