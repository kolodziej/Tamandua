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
#include <thread>
#include <utility>
#include <functional>
#include <condition_variable>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

using boost::asio::ip::tcp;

namespace tamandua
{
	typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket_stream;

	class client
	{
		public:
			typedef enum {
				connecting_succeeded = 1,
				connecting_failed,
				message_sent,
				message_undelivered,
				message_received,
				server_disconnected,
				initialization_data_received,
				participants_list_received,
				rooms_list_received,
			} event_type;

		private:
			id_number_t uid_;
			boost::asio::io_service io_service_;
			std::thread io_service_thread_;
			tcp::resolver resolver_;
			tcp::resolver::iterator endpoint_iterator_;
			boost::asio::ssl::context &context_;
			ssl_socket_stream socket_;
			std::map<id_number_t, std::string> participants_;
			std::map<id_number_t, std::string> rooms_;
			std::deque<message> messages_;
			std::mutex messages_queue_lock_;
			std::condition_variable new_message_cv_;
			message read_message_;
			bool connected_;
		
			std::map<event_type, std::function<void(status)>> events_handlers_;

		public:
			client(tcp::resolver::iterator endpoint_iterator, boost::asio::ssl::context &context) :
				resolver_(io_service_),
				endpoint_iterator_(endpoint_iterator),
				context_(context),
				socket_(io_service_, context),
				connected_(false)
			{
				connect_(endpoint_iterator);
				run_io_service_thread_();
			}

			client(boost::asio::ssl::context &context) :
				resolver_(io_service_),
				context_(context),
				socket_(io_service_, context),
				connected_(false)
			{}

			void connect(std::string, std::string);
			void disconnect();
			bool is_connected()
			{
				return connected_;
			}

			id_number_t get_id();
			ssl_socket_stream &get_socket();
			const std::map<id_number_t, std::string>& get_participants_list();
			const std::map<id_number_t, std::string>& get_rooms_list();

			void send_message(std::string &, id_number_t = 0);
			bool is_next_message();
			std::pair<std::string, message> get_next_message();

			void add_event_handler(event_type, std::function<void(status)>);

		private:
			void connect_(tcp::resolver::iterator);
			void run_io_service_thread_();
			void add_message_();
			void add_message_(message &&);
			void add_message_(message_type, std::string &);
			void add_message_(message_type, std::string &&);
			void write_message_(message &);
			void perform_handshake_();
			void read_message_header_();
			void read_message_body_();
			void process_message_();
			void set_participants_list_();
			void set_rooms_list_();
			void send_quit_message_();

			bool is_event_handler_(event_type);
			void call_event_handler_(event_type, status);
			void server_disconnected_();
	};
}

#endif
