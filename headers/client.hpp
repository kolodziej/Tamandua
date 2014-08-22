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
			void connect(std::string host, std::string port, Callback f)
			{
				tcp::resolver resolver(io_service_);
				tcp::resolver::iterator endpoint_it = resolver.resolve({ host, port });
				connect(endpoint_it, f);
			}

			template <typename Callback>
			void connect(tcp::resolver::iterator endpoint_iterator, Callback f)
			{
				endpoint_iterator_ = endpoint_iterator;
				boost::asio::async_connect(socket_, endpoint_iterator_,
					[this, f](boost::system::error_code ec, tcp::resolver::iterator iterator)
					{
						TamanduaDebug("Connect callback run");
						if (ec)
							f(connection_failed);
						else
							f(ok);

						read_message_header_();
					});
			}

			id_number_t get_id();

			template <typename Callback>
			void send_message(message & msg, Callback f)
			{
				msg.header.author = uid_;
				msg.header.id = 0;
				msg.header.type = message_type::standard_message;
				msg.header.size = msg.body.length();
				message_buffer buf(msg.header, msg.body);
				boost::asio::async_write(socket_,
					boost::asio::buffer(buf.get_buffer().get(), buf.get_buffer_size()),
					[this, f](boost::system::error_code ec, size_t length)
					{
						if (ec)
							f(message_undelivered);
						else
							f(ok);
					});
			}

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
