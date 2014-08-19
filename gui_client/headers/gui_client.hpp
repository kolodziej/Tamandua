#ifndef GUI_CLIENT_HPP
#define GUI_CLIENT_HPP
#include "wx/wx.h"
#include "main_frame.hpp"

class gui_client :
	public wxApp
{
	private:
		main_frame *frame;
	public:
		virtual bool OnInit();

};

#endif
