#ifndef GUI_CLIENT_HPP
#define GUI_CLIENT_HPP
#include "wx/wx.h"
#include "tamandua.hpp"
#include <thread>
#include <mutex>
#include <memory>
#include <boost/asio.hpp>

class main_frame;

class gui_client :
	public wxApp
{
	private:
		std::mutex *main_lock_;
		bool running_;
		boost::asio::io_service *io_service_;
		tamandua::client *client_;
		main_frame *frame;
		std::thread reader_thread, io_service_thread;
	public:
		virtual bool OnInit();
		tamandua::client * get_client();
		void io_service_run();

		virtual int OnExit();

};

#endif
