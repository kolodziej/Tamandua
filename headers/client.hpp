#ifndef TAMANDUA_CLIENT_HPP
#define TAMANDUA_CLIENT_HPP
#include "types.hpp"
#include "message.hpp"
#include <map>
#include <deque>
#include <string>
#include <mutex>
#include <utility>
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
			message read_message_;

		public:
			template <typename Callback>
			client(boost::asio::io_service &io_service, tcp::resolver::iterator endpoint_iterator, Callback f) :
				io_service_(io_service),
				endpoint_iterator_(endpoint_iterator),
				socket_(io_service)
			{
				connect(endpoint_iterator, f);
			}

			client(boost::asio::io_service &io_service) :
				io_service_(io_service),
				socket_(io_service)
			{}
			
			template <typename Callback>
			void connect(std::string, std::string, Callback);

			template <typename Callback>
			void connect(tcp::resolver::iterator, Callback);

			id_number_t get_id();

			template <typename Callback>
			void send_message(message &, Callback);

			bool is_next_message();
			std::pair<std::string, message> get_next_message();

		private:
			void add_message_();
			void add_message_(message_type, std::string &);
			void add_message_(message_type, std::string &&);
			void read_message_header_();
			void read_message_body_();
			void process_message_();
			void set_participants_list_();
			void set_rooms_list_();
	};
}

#endif
