#ifndef TAMANDUA_SERVER_HPP
#define TAMANDUA_SERVER_HPP
#include "types.hpp"
#include "logger.hpp"
#include <map>
#include <string>
#include <memory>
#include <boost/asio.hpp>

#define ServerLog(...) stream_log(log_stream_, __VA_ARGS__)

using boost::asio::ip::tcp;

namespace tamandua
{
	class participant;
	class group;

	class server
	{
		private:
			boost::asio::io_service &io_service_;
			tcp::acceptor acceptor_;
			tcp::socket socket_;
			tcp::endpoint endpoint_;
			logger &log_;

			std::map<id_number_t, std::shared_ptr<participant>> participants_;
			std::map<id_number_t, std::shared_ptr<group>> groups_;

			id_number_t last_participant_id_, last_group_id_, last_message_id_;

		public:
			server(boost::asio::io_service &, tcp::endpoint &, logger &);
			~server();
		
			void start_server();
			void process_message();

			void add_participant(std::shared_ptr<participant>);
			void add_group(std::shared_ptr<group>);

			id_number_t get_last_participant_id() const;
			id_number_t get_last_group_id() const;
			id_number_t get_last_message_id() const;

		private:
			void add_root_();
			void add_new_user_();
			void accept_connection_();
			void send_init_message_(std::shared_ptr<participant>);
			void send_to_all_(message_type, std::string);

			friend class participant;
			id_number_t get_new_participant_id_();
			friend class group;
			id_number_t get_new_group_id_();
			id_number_t get_new_message_id_();

			std::string get_default_user_name_(id_number_t);
	};
}

#endif
