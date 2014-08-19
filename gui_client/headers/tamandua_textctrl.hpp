#ifndef TAMANDUA_TEXTCTRL_HPP
#define TAMANDUA_TEXTCTRL_HPP
#include "wx/wx.h"
#include "wx/textctrl.h"

class tamandua_textctrl :
	public wxTextCtrl
{
	public:
		tamandua_textctrl(wxWindow *win, wxWindowID id) :
			wxTextCtrl(win, id, wxEmptyString, wxPoint(0,0), wxDefaultSize, wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_READONLY | wxTE_AUTO_URL)
		{}
};

#endif
