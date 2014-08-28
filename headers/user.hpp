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
		public participant,
		public std::enable_shared_from_this<user>
	{
		private:
			tcp::socket socket_;
			std::deque<message> messages_queue_;
			std::shared_ptr<group> group_;
			message read_message_;
			status quit_status_;
			
		public:
			user(server &svr, std::string & name, tcp::socket socket) : participant(svr, name), socket_(std::move(socket)), quit_status_(ok)
			{
				read_message();		
			}
			user(server &svr, std::string && name, tcp::socket socket) : participant(svr, name), socket_(std::move(socket)), quit_status_(ok)
			{
				read_message();	
			}
			~user();

			std::string get_ip_address();
			
			virtual void read_message();
			virtual void deliver_message(const message&);

			// commands
			void cmd_id(std::string &);
			void cmd_room(std::string &);
			void cmd_proom(std::string &);
			void cmd_nick(std::string &);
			void cmd_msg(std::string &);
			void cmd_kick_ass(std::string &);
			void cmd_server_uptime(std::string &);
			void cmd_quit(std::string &);

		private:
			void read_control_sequence_();
			void interprete_control_sequence_(char);
			void read_message_header_();
			void read_message_body_();
			void process_message_();
			void send_messages_();

			void quit_();
			void error_quit_();

	};
}

#endif
