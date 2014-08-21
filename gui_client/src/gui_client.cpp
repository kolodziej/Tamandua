#include "gui_client.hpp"
#include "main_frame.hpp"
#include "tamandua.hpp"
#include "debug_gui.hpp"
#include <thread>
#include <boost/asio.hpp>

bool gui_client::OnInit()
{
	io_service_ = new boost::asio::io_service;
	client_ = new tamandua::client(*io_service_);
	frame = new main_frame();

	reader_thread = new std::thread([this]() {
		bool running = true;
		tamandua::client &cl = *client_;
		while (running)
		{
			if (cl.is_next_message())
			{
				Debug("adding new message!");
				auto msg_pair = cl.get_next_message();
				wxString author(msg_pair.first.c_str(), wxConvUTF8);
				wxString msg_body(msg_pair.second.body.c_str(), wxConvUTF8);
				tamandua::message &msg = msg_pair.second;
				switch (msg.header.type)
				{
					case message_type::info_message:
						frame->get_msgs()->add_info(msg_body);
						break;

					case message_type::error_message:
						frame->get_msgs()->add_error(msg_body);
						break;
					
					default:
						frame->get_msgs()->add_message(author, msg_body);
						break;
				}
			}
		}
	});

	frame->Show();
	Debug("Returning OnInit");
	return true;
}

tamandua::client * gui_client::get_client()
{
	return client_;
}

int gui_client::OnExit()
{
	reader_thread->join();
	return 0;
}
