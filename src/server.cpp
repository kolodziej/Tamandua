#include "server.hpp"
#include "root.hpp"
#include "message.hpp"
#include "user.hpp"
#include <sstream>

using namespace tamandua;

server::server(boost::asio::io_service &io_service, tcp::endpoint &endpoint, std::ostream &log_stream) :
	io_service_(io_service),
	acceptor_(io_service, endpoint),
	socket_(io_service_),
	endpoint_(endpoint),
	log_stream_(log_stream)
{
	add_participant(std::shared_ptr<participant>(new root));

	accept_connection();
}

server::~server()
{}

void server::accept_connection()
{
	acceptor_.async_accept(socket_,
	[this](boost::system::error_code ec)
	{
		if (!ec)
		{
			id_number_t uid = get_new_participant_id();
			std::stringstream stream;
			stream << "User" << uid;
			std::shared_ptr<participant> usr(new user(uid, stream.str(), std::move(socket_)));
			add_participant(usr);
			send_init_message_(usr);
		}
	});
}

void server::process_message()
{

}

void server::add_participant(std::shared_ptr<participant> pt)
{
	participants_.insert(std::pair<id_number_t, std::shared_ptr<participant>>(pt->get_id(), pt));
}

void server::add_group(std::shared_ptr<group> gr)
{
	groups_.insert(std::pair<id_number_t, std::shared_ptr<group>>(gr->get_id(), gr));
}

id_number_t server::get_new_participant_id()
{
	return ++last_participant_id_;
}

id_number_t server::get_new_group_id()
{
	return ++last_group_id_;
}

id_number_t server::get_new_message_id()
{
	return ++last_message_id_;
}

id_number_t server::get_last_participant_id() const
{
	return last_participant_id_;
}

id_number_t server::get_last_group_id() const
{
	return last_group_id_;
}

id_number_t server::get_last_message_id() const
{
	return last_message_id_;
}

void server::send_init_message_(std::shared_ptr<participant> usr)
{
	message_header header;
	header.id = get_new_message_id();
	header.author = usr->get_id();
	header.type = message_type::init_message;
	header.size = usr->get_name().length();
	std::string body = usr->get_name();
	usr->deliver_message(message(header, body));
	accept_connection();
}
