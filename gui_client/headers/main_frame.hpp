#ifndef MAIN_FRAME_HPP
#define MAIN_FRAME_HPP
#include "wx/wx.h"
#include "wx/textctrl.h"
#include "tamandua_textctrl.hpp"
#include "gui_client.hpp"
#include <initializer_list>

class main_frame :
	public wxFrame
{
	private:
		gui_client &app;

		wxPanel *panel;
		tamandua_textctrl *msgs;
		wxTextCtrl *msg, *connect_host, *connect_port;
		wxButton *connect_button;
		wxBoxSizer *sizer;
		wxBoxSizer *connect_sizer;
	public:
		enum {
			MSGS_CTRL = 1000,
			MSG_CTRL,
			PANEL,
			SIZER,
			CON_HOST_TEXT,
			CON_PORT_TEXT,
			CON_BTN
		};

		main_frame(gui_client &);

		void send_message(wxCommandEvent &);
		void connect(wxCommandEvent &);

	private:
		DECLARE_EVENT_TABLE();

};

#endif
