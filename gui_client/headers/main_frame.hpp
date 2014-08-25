#ifndef MAIN_FRAME_HPP
#define MAIN_FRAME_HPP
#include "wx/wx.h"
#include "wx/textctrl.h"
#include "tamandua_textctrl.hpp"
#include "gui_client.hpp"
#include "tamandua_box.hpp"
#include <initializer_list>

class main_frame :
	public wxFrame
{
	private:
		wxPanel *panel;
		tamandua_textctrl *msgs;
		wxTextCtrl *msg, *connect_host, *connect_port;
		wxButton *connect_button;
		wxBoxSizer *sizer;
		wxBoxSizer *connect_sizer;

		bool connected;
		tamandua_box *tb;
	public:
		enum {
			MSGS_CTRL = 1000,
			MSG_CTRL,
			PANEL,
			SIZER,
			CON_HOST_TEXT,
			CON_PORT_TEXT,
			CON_BTN,
			FUNNY_BTN
		};

		main_frame();

		void connect(wxCommandEvent &);
		void send_message(wxCommandEvent &);
		void disconnect(wxCommandEvent &);
		void msgs_url(wxTextUrlEvent &);

		tamandua_textctrl *get_msgs()
		{
			return msgs;
		}

	private:
		DECLARE_EVENT_TABLE();

		void connecting_succeeded_();
		void connecting_failed_();
		void message_sent_();
		void message_undelivered_();

};

#endif
