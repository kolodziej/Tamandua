#ifndef TAMANDUA_CLIENT_HPP
#define TAMANDUA_CLIENT_HPP
#include "types.hpp"
#include "message.hpp"
#include "message_buffer.hpp"
#include "logger.hpp"
#include <map>
#include <deque>
#include <string>
#include <mutex>
#include <utility>
#include <functional>
#include <condition_variable>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace tamandua
{
	class client
	{
		private:
			id_number_t uid_;
			boost::asio::io_service &io_service_;
			tcp::resolver::iterator endpoint_iterator_;
			tcp::socket socket_;

			std::map<id_number_t, std::string> participants_;
			std::map<id_number_t, std::string> rooms_;
			std::deque<message> messages_;
			std::mutex messages_queue_lock_;
			std::condition_variable new_message_cv_;
			message read_message_;
			bool connected_;
		
			std::map<event_type, std::function<void(status)>> events_handlers_;

		public:
			client(boost::asio::io_service &io_service, tcp::resolver::iterator endpoint_iterator) :
				io_service_(io_service),
				endpoint_iterator_(endpoint_iterator),
				socket_(io_service),
				connected_(false)
			{
				connect(endpoint_iterator);
			}

			client(boost::asio::io_service &io_service) :
				io_service_(io_service),
				socket_(io_service),
				connected_(false)
			{}

			void connect(std::string, std::string);
			void connect(tcp::resolver::iterator);
			void disconnect();
			bool is_connected()
			{
				return connected_;
			}

			id_number_t get_id();

			void send_message(message &);
			bool is_next_message();
			std::pair<std::string, message> get_next_message();

			void add_event_handler(event_type, std::function<void(status)>);

		private:
			void add_message_();
			void add_message_(message_type, std::string &);
			void add_message_(message_type, std::string &&);
			void read_message_header_();
			void read_message_body_();
			void process_message_();
			void set_participants_list_();
			void set_rooms_list_();

			bool is_event_handler_(event_type);
			void call_event_handler_(event_type, status);
			void server_disconnected_();
	};
}

#endif
