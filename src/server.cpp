#include "server.hpp"
#include "logger.hpp"
#include "root.hpp"
#include "message.hpp"
#include "message_composer.hpp"
#include "user.hpp"
#include "room.hpp"
#include "utility.hpp"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>

using namespace tamandua;

server::server(boost::asio::io_service &io_service, tcp::endpoint &endpoint, logger &log, command_interpreter &interpreter, boost::asio::ssl::context &context) :
	io_service_(io_service),
	context_(std::move(context)),
	acceptor_(io_service, endpoint),
	endpoint_(endpoint),
	log_(log),
	interpreter_(interpreter),
	last_participant_id_(0),
	last_group_id_(0)
{
}

server::~server()
{}

void server::start_server(server_config &config)
{
	using std::chrono::system_clock;
	start_time_ = system_clock::now();
	Log(log_, "Starting server at: ", format_localtime<system_clock, 30>(start_time_, "%c"));
	add_root_(config.root_password);
	add_hall_(config.main_room_name);
	accept_connection_();
}

void server::send_startup_data(std::shared_ptr<participant> usr)
{
	message_composer msgc(message_type::init_message, usr->get_id());
	msgc << usr->get_name();
	usr->deliver_message(msgc());

	send_participants_list_();
	send_rooms_list_();
	accept_connection_();
}

boost::asio::io_service &server::get_io_service()
{
	return io_service_;
}

void server::process_message(std::shared_ptr<user> pt, message &msg)
{
	if (msg.header.type == message_type::quit_message)
	{
		quit_participant(pt->get_id(), ok);
		return;
	}
	// user message interpreter
	processing_status msg_stat = interpreter_.process(pt ,msg);

	if (msg_stat == processing_status::std_msg)
	{
		if (pt->group_)
			pt->group_->deliver_message(msg);
		else
		{
			message_composer msgc(message_type::error_message);
			msgc << "You must select group using /room <name> or /proom <name> <password>. Your client received full list of public rooms.";
			pt->deliver_message(msgc());
		}
	} else if (msg_stat == processing_status::bad_cmd)
	{
		message_composer msgc(message_type::error_message);
		msgc << "Unknown command: [" << msg.body << "]!";// Available commands: " << get_interpreter().get_commands_list();
		pt->deliver_message(msgc());
	} else if (msg_stat == processing_status::cmd_processed)
	{
		Log(log_, "Command `", msg.body, "` processed!");
	} else
	{
		Error(log_, "Unknown processing_status for message: '", msg.body, "'!");
	}
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

bool server::change_participant_name(std::string oldname, std::string newname)
{
	if (is_participant_name_available(newname))
	{
		auto ptr = get_participant(oldname);
		id_number_t id = ptr->get_id();
		participants_ids_.erase(oldname);
		participants_.erase(id);
		participants_ids_.insert(make_pair(newname, id));
		participants_.insert(make_pair(id, ptr));
		ptr->set_name(newname);
		send_participants_list_();
		return true;
	}

	return false;
}

void server::quit_participant(id_number_t uid, status st)
{
	std::string username;
	auto u = participants_.find(uid);
	if (u != participants_.end())
	{
		username = (*u).second->get_name();
		participants_.erase(u);
	}

	auto u_id = participants_ids_.find(username);
	if (u_id != participants_ids_.end())
		participants_ids_.erase(u_id);

	message_composer msgc(message_type::info_message);
	msgc << "User " << username << " is quitting server";
	if (st == ok)
		msgc << "!";
	else if (st == user_error_quit)
		msgc << " [due to client side error occurance]!";

	message quit_info = msgc();
	Log(log_, quit_info.body);
	for (auto part : participants_)
		part.second->deliver_message(quit_info);

	send_participants_list_();
}

std::shared_ptr<participant> server::get_participant(id_number_t id)
{
	auto it = participants_.find(id);
	if (it != participants_.end())
		return (*it).second;

	return nullptr;
}

std::shared_ptr<participant> server::get_participant(std::string name)
{
	auto id_it = participants_ids_.find(name);
	if (id_it != participants_ids_.end())
		return get_participant((*id_it).second);

	return nullptr;
}

std::shared_ptr<group> server::get_group(id_number_t id)
{
	auto it = groups_.find(id);
	if (it != groups_.end())
		return (*it).second;

	return nullptr;
}

std::shared_ptr<group> server::get_group(std::string name)
{
	auto id_it = groups_ids_.find(name);
	if (id_it != groups_ids_.end())
		return get_group((*id_it).second);

	return nullptr;
}

bool server::is_participant_name_available(std::string name)
{
	return (participants_ids_.find(name) == participants_ids_.end());
}

bool server::is_group_name_available(std::string name)
{
	return (groups_ids_.find(name) == groups_ids_.end());
}

id_number_t server::get_last_participant_id() const
{
	return last_participant_id_;
}

id_number_t server::get_last_group_id() const
{
	return last_group_id_;
}

bool server::is_username_valid(std::string username)
{
	if (username.length() < 2)
		return false;

	return true;
}

std::string server::get_uptime_string()
{
	std::chrono::duration<long int> uptime_seconds = std::chrono::duration_cast<std::chrono::duration<long int>>(std::chrono::system_clock::now() - start_time_);
	long int ticks = uptime_seconds.count();
	int d, h, m, s;
	s = ticks % 60;
	ticks /= 60;
	m = ticks % 60;
	ticks /= 60;
	h = ticks % 24;
	ticks /= 24;
	d = ticks;
	std::stringstream stream;
	stream << std::setw(3) << std::setfill('0') << d << " days, " << std::setw(2) << std::setfill('0') << h << ":" << std::setw(2) << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0') << s;
	return stream.str();
}

logger & server::get_logger()
{
	return log_;
}

command_interpreter & server::get_interpreter()
{
	return interpreter_;
}

void server::add_root_(std::string password)
{
	add_participant(std::shared_ptr<participant>(new root(*this, password)));
}

void server::add_hall_(std::string name)
{
	add_group(std::shared_ptr<group>(new room(*this, name)));
}

void server::accept_connection_()
{
	std::shared_ptr<user> new_user(new user(*this, std::string(), context_));
	acceptor_.async_accept(new_user->get_socket(),
	[this, new_user](boost::system::error_code ec)
	{
		if (!ec)
		{
			Log(log_, "Accepted connection of new user from IP: ",new_user->get_socket().remote_endpoint().address().to_string());
			add_participant(new_user);
			new_user->start();
		}
	});
	Log(log_, "Waiting for connection");
}


void server::send_rooms_list_()
{
	message_header header;
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
