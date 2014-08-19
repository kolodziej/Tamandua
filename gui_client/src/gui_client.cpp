#include "gui_client.hpp"
#include "main_frame.hpp"
#include "tamandua.hpp"
#include <thread>
#include <boost/asio.hpp>

bool gui_client::OnInit()
{
	io_service_ = new boost::asio::io_service;
	client_ = new tamandua::client(*io_service_);
	frame = new main_frame(*this);
	frame->Show();
	return true;
}

tamandua::client * gui_client::get_client()
{
	return client_;
}
