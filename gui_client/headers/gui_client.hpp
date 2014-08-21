#ifndef GUI_CLIENT_HPP
#define GUI_CLIENT_HPP
#include "wx/wx.h"
#include "tamandua.hpp"
#include <thread>
#include <boost/asio.hpp>

class main_frame;

class gui_client :
	public wxApp
{
	private:
		boost::asio::io_service *io_service_;
		tamandua::client *client_;
		main_frame *frame;
		std::thread *reader_thread;
	public:
		virtual bool OnInit();
		tamandua::client * get_client();

		virtual int OnExit();

};

#endif
