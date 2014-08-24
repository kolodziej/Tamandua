#include <thread>
#include <boost/asio.hpp>
#include "gui_client.hpp"
#include "main_frame.hpp"
#include "tamandua.hpp"
#include "debug_gui.hpp"
#include <functional>
#include <chrono>

bool gui_client::OnInit()
{
	main_lock_ = new std::mutex;
	io_service_ = new boost::asio::io_service;
	client_ = new tamandua::client(*io_service_);
	frame = new main_frame();
	running_ = true;
	
	reader_thread = std::thread([this]() {
		tamandua::client &cl = *client_;
		bool local_running = true;
		do {
			if (cl.is_next_message())
			{
				auto msg_pair = cl.get_next_message();
				wxString author = wxString::FromUTF8(msg_pair.first.data());
				wxString msg_body = wxString::FromUTF8(msg_pair.second.body.data());
				tamandua::message &msg = msg_pair.second;
				switch (msg.header.type)
				{
					case tamandua::message_type::info_message:
						Debug("info: ", msg_body.utf8_str());
						wxTheApp->GetTopWindow()->GetEventHandler()->CallAfter(std::bind(&tamandua_textctrl::add_info, frame->get_msgs(), msg_body));
						break;

					case tamandua::message_type::error_message:
						Debug("error: ", msg_body.utf8_str());
						wxTheApp->GetTopWindow()->GetEventHandler()->CallAfter(std::bind(&tamandua_textctrl::add_error, frame->get_msgs(), msg_body));
						break;
					
					default:
						Debug("<", author, ">: ", msg_body.utf8_str()); 
						wxTheApp->GetTopWindow()->GetEventHandler()->CallAfter(std::bind(&tamandua_textctrl::add_message, frame->get_msgs(), author, msg_body));
						break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			main_lock_->lock();
			local_running = running_;
			main_lock_->unlock();
		} while (local_running);
	});
	frame->Show();
	return true;
}

void gui_client::io_service_run()
{
	io_service_thread = std::thread([this]() {
		Debug("Running io_service::run()");
		io_service_->run();
		Debug("io_service::run() returned");
	});
}

tamandua::client * gui_client::get_client()
{
	return client_;
}

int gui_client::OnExit()
{
	client_->disconnect();
	main_lock_->lock();
	running_ = false;
	main_lock_->unlock();
	reader_thread.join();
	io_service_thread.join();
	delete main_lock_;
	delete client_;
	delete io_service_;
	return 0;
}
