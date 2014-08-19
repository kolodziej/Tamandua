#include "server.hpp"
#include "logger.hpp"
#include "root.hpp"
#include "message.hpp"
#include "user.hpp"
#include <sstream>

using namespace tamandua;

server::server(boost::asio::io_service &io_service, tcp::endpoint &endpoint, logger &log, user_message_interpreter &interp) :
	io_service_(io_service),
	acceptor_(io_service, endpoint),
	socket_(io_service),
	endpoint_(endpoint),
	log_(log),
	interpreter_(interp),
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
	add_hall_();
	accept_connection_();
}

void server::add_participant(std::shared_ptr<participant> pt)
{
	std::string username = pt->get_name();
	auto id_it = participants_ids_.find(username);
	if (id_it == participants_ids_.end())
	{
		participants_.insert(make_pair(pt->get_id(), pt));
		participants_ids_.insert(make_pair(username, pt->get_id()));
		Log(log_, "Added new participant ID: ",pt->get_id());
	} else
	{
		Error(log_, "Could not add new participant! Name '", username, "' is already in use!");
	}
}

void server::add_group(std::shared_ptr<group> gr)
{
	std::string groupname = gr->get_name();
	auto id_it = groups_ids_.find(groupname);
	if (id_it == groups_ids_.end())
	{
		groups_.insert(make_pair(gr->get_id(), gr));
		groups_ids_.insert(make_pair(groupname, gr->get_id()));
		Log(log_, "Added new group ID: ",gr->get_id());
	} else
	{
		Error(log_, "Could not add new group! Name '", groupname, "' is already in use!");
	}
}

void server::quit_user(id_number_t uid)
{
	auto u = participants_.find(uid);
	message quit_msg(message_type::quit_message, std::string());
	std::string username((*u).second->get_name());
	(*u).second->deliver_message(quit_msg);

	if (u != participants_.end())
		participants_.erase(u);

	std::stringstream stream;
	stream << "User " << username << " is quitting server!";
	message quit_info(message_type::info_message, stream.str());
	for (auto part : participants_)
		part.second->deliver_message(quit_info);

	send_participants_list_();
}

std::shared_ptr<participant> server::get_participant(id_number_t id)
{
	auto it = participants_.find(id);
	if (it != participants_.end())
		return *it.second;

	return nullptr;
}

std::shared_ptr<participant> server::get_participant(std::string name)
{
	auto id_it = participants_ids_.find(name);
	if (id_it != participants_ids_.end())
		return get_participant(*id_it.second);

	return nullptr;
}

std::shared_ptr<group> server::get_group(id_number_t id)
{
	auto it = groups_.find(id);
	if (it != groups_.end())
		return *it.second;

	return nullptr;
}

std::shared_ptr<group> server::get_group(std::string name)
{
	auto id_it = groups_ids_.find(name);
	if (id_it != groups_ids_.end())
		return get_group(*id_it.second);

	return nullptr;
}
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

logger & server::get_logger()
{
	return log_;
}

user_message_interpreter & server::get_interpreter()
{
	return interpreter_;
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
	add_participant(std::shared_ptr<participant>(new root(*this, "dd"))); // test password
}

void server::add_hall_()
{
	add_group(std::shared_ptr<group>(new room(*this, "Hall"));
}

void server::add_new_user_()
{
	std::shared_ptr<participant> usr(new user(*this, std::string(), std::move(socket_)));
	add_participant(usr);
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

	send_participants_list_();
	send_rooms_list_();
	accept_connection_();
}

void server::send_rooms_list_()
{
	message_header header;
	header.id = get_new_message_id_();
	header.author = 0;
	header.type = message_type::rooms_list;
	std::string rooms_list = generate_rooms_list_();
	header.size = rooms_list.size();
	for (auto &part : participants_)
		part.second->deliver_message(message(header, rooms_list));
}

void server::send_participants_list_()
{
	message_header header;
	header.id = get_new_message_id_();
	header.author = 0;
	header.type = message_type::participants_list;
	std::string pt_list = generate_participants_list_();
	header.size = pt_list.size();
	for (auto &part : participants_)
		part.second->deliver_message(message(header, pt_list));
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

std::string server::get_default_user_name_(id_number_t id)
{
	std::stringstream username;
	username << "User#" << id;
	return username.str();
}

std::string server::get_default_group_name_(id_number_t id)
{
	std::stringstream groupname;
	groupname << "Group#" << id;
	return groupname.str();
}

std::string server::generate_rooms_list_()
{
	std::stringstream stream;
	for (auto &room : groups_)
	{
		if (room.second->is_hidden() == false)
			stream << room.first << ":" << room.second->get_name() << ";";
	}

	return stream.str();
}

std::string server::generate_participants_list_()
{
	std::stringstream stream;
	for(auto &part : participants_)
	{
		if (part.second->is_hidden() == false)
			stream << part.first << ":" << part.second->get_name() << ";";
	}
	return stream.str();
}
