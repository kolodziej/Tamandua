#include "message_composer.hpp"

using namespace tamandua;

message & message_composer::get_msg()
{
	prepare_message_();
	cleanup_();
	return msg_;
}

void message_composer::prepare_message_()
{
	msg_.body = stream_.str();
	msg_.header.id = msg_id_;
	msg_.header.author = u_id_;
	msg_.header.type = type_;
	msg_.header.size = msg_.body.length();
}

void message_composer::cleanup_()
{
	stream_.str(std::string());
}
