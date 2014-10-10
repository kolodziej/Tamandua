#include "user.hpp"
#include "message_buffer.hpp"
#include "message_composer.hpp"
#include "room.hpp"
#include "private_room.hpp"
#include <utility>
#include <iostream>
#include <cmath>
#include <boost/program_options.hpp>

using namespace tamandua;

user::~user()
{
	Log(get_server().get_logger(), "User ID ", get_id(), " is leaving server!");
}

session& user::get_session()
{
	return session_;
}

void user::read_message()
{
	if (quit_status_ != ok)
	{
		if (quit_status_ == user_quit)
			quit_();
		else if (quit_status_ == user_error_quit)
			error_quit_();

		return;
	}	
}

void user::deliver_message(const message &message)
{
	if (quit_status_ != ok)
		return;

	bool start_sending = messages_queue_.empty();
	messages_queue_.push_back(message);
	// deliver message from session instance
}

void user::lock(unsigned int minutes, std::string reason)
{
	locked_until_ = message_time_clock_t::now() + std::chrono::minutes(minutes);
	message_composer msgc(warning_message);
	msgc << "You have been locked for " << minutes << " minutes. ";
	if (reason.empty() == false)
		msgc << "(reason: " << reason << ")";

	deliver_message(msgc());
}

void user::unlock(std::string reason)
{
	locked_until_ = message_time_clock_t::now() - std::chrono::seconds(1);
	message_composer msgc(info_message);
	msgc << "You have been unlocked! ";
	if (reason.empty() == false)
		msgc << "(reason: " << reason << ")";

	deliver_message(msgc());
}

bool user::is_locked()
{
	return (locked_until_ < message_time_clock_t::now());
}

void user::remove(std::string reason)
{
	message_composer msgc(warning_message);
	msgc << "You have been removed from server by root! ";
	if (reason.empty() == false)
		msgc << "(reason: " << reason << ")";

	deliver_message(msgc());
	quit_();
}

void user::quit()
{
	quit_status_ = user_quit;
}

void user::add_to_hall_()
{
	get_server().get_group(0)->join_participant(shared_from_this());
	add_group(0);
}

void user::quit_()
{
	try {
		quit_status_ = user_quit;
		//socket_.lowest_layer().cancel();
		//socket_.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		//socket_.shutdown();
		get_server().quit_participant(std::dynamic_pointer_cast<participant>(shared_from_this()), ok);
	} catch (boost::system::system_error &err)
	{
		Error(get_server().get_logger(), "An error occurred while user quitting: ", err.what());
	}
}

void user::error_quit_()
{
	quit_status_ = user_error_quit;
	get_server().quit_participant(std::dynamic_pointer_cast<participant>(shared_from_this()), user_error_quit);
}

