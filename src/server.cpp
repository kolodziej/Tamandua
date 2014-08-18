#include "server.hpp"
#include "logger.hpp"
#include "root.hpp"
#include "message.hpp"
#include "user.hpp"

using namespace tamandua;

server::server(boost::asio::io_service &io_service, tcp::endpoint &endpoint, logger &log) :
	io_service_(io_service),
	acceptor_(io_service, endpoint),
	socket_(io_service),
	endpoint_(endpoint),
	log_(log),
	last_participant_id_(0),
	last_group_id_(0),
	last_message_id_(0)
{}

server::~server()
{}

void server::start_server()
{
	Log(log_, "Starting server at: ");
	add_root_();
	Log(log_, "Root user joined!");
	accept_connection_();
}

void server::process_message()
{

}

void server::add_participant(std::shared_ptr<participant> pt)
{
	participants_.insert(std::pair<id_number_t, std::shared_ptr<participant>>(pt->get_id(), pt));
	Log(log_, "Added new participant ID: ",pt->get_id());
}

void server::add_group(std::shared_ptr<group> gr)
{
	groups_.insert(std::pair<id_number_t, std::shared_ptr<group>>(gr->get_id(), gr));
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

void server::accept_connection_()
{
	acceptor_.async_accept(socket_,
	[this](boost::system::error_code ec)
	{
		if (!ec)
		{
			Log(log_, "Accepted connection of new user from IP: ",socket_.remote_endpoint().address().to_string());
			add_new_user_();
		}
	});
	Log(log_, "Waiting for connection");
}

void server::add_root_()
{
	add_participant_(std::shared_ptr<participant>(new root(*this)));
}

void server::add_new_user_()
{
	std::shared_ptr<participant> usr(new user(*this, std::string(), std::move(socket_)));
	add_participant_(usr);
	send_init_message_(usr);
}

void server::send_init_message_(std::shared_ptr<participant> usr)
{
	message_header header;
	header.id = get_new_message_id_();
	header.author = usr->get_id();
	header.type = message_type::init_message;
	header.size = usr->get_name().length();
	std::string body = usr->get_name();
	usr->deliver_message(message(header, body));
	Log(log_, "Init message for user ID ",usr->get_id()," has been queued for delivery");
	accept_connection_();
}

id_number_t server::get_new_participant_id_()
{
	return last_participant_id_++;
}

id_number_t server::get_new_group_id_()
{
	return last_group_id_++;
}

id_number_t server::get_new_message_id_()
{
	return last_message_id_++;
}
