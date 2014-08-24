#include <thread>
#include <boost/asio.hpp>
#include "gui_client.hpp"
#include "main_frame.hpp"
#include "tamandua.hpp"
#include "debug_gui.hpp"

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
				wxString author(msg_pair.first.c_str(), wxConvUTF8);
				wxString msg_body(msg_pair.second.body.c_str(), wxConvUTF8);
				tamandua::message &msg = msg_pair.second;
				switch (msg.header.type)
				{
					case tamandua::message_type::info_message:
						Debug("info: ", msg_body);
						frame->get_msgs()->add_info(wxString(msg_body));
						break;

					case tamandua::message_type::error_message:
						Debug("error: ", msg_body);
						frame->get_msgs()->add_error(wxString(msg_body));
						break;
					
					default:
						Debug("<", author, ">: ", msg_body); 
						frame->get_msgs()->add_message(wxString(author), wxString(msg_body));
						break;
				}
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
		std::cout << "Running io_service::run()\n";
		io_service_->run();
		std::cout << "io_service::run() returned\n";
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
