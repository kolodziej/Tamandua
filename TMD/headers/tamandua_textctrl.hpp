#ifndef TAMANDUA_TEXTCTRL_HPP
#define TAMANDUA_TEXTCTRL_HPP
#include "wx/wx.h"
#include "wx/textctrl.h"
#include <mutex>

class tamandua_textctrl :
	public wxTextCtrl
{
	private:
		std::mutex write_lock_;
	public:
		tamandua_textctrl(wxWindow *win, wxWindowID id) :
			wxTextCtrl(win, id, wxEmptyString, wxPoint(0,0), wxDefaultSize, wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_READONLY | wxTE_AUTO_URL)
		{}

		void add_message(wxString, wxString);
		void add_info(wxString);
		void add_error(wxString);
		void add_warning(wxString);
		void add_private_message(wxString, wxString);
};

#endif
