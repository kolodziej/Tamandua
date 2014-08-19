#ifndef MAIN_FRAME_HPP
#define MAIN_FRAME_HPP
#include "wx/wx.h"
#include "wx/textctrl.h"
#include "tamandua_textctrl.hpp"
#include <initializer_list>

class gui_client;

class main_frame :
	public wxFrame
{
	private:
		gui_client &app;

		wxPanel *panel;
		tamandua_textctrl *msgs;
		wxTextCtrl *msg;
		wxBoxSizer *sizer;
	public:
		enum {
			MSGS_CTRL = 1000,
			MSG_CTRL,
			PANEL,
			SIZER
		};

		main_frame(gui_client &);


};

#endif
