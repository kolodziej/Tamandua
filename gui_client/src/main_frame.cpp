#include "main_frame.hpp"

main_frame::main_frame(gui_client &app_) :
	wxFrame(0, wxID_ANY, wxT("Tamandua GUI Client")),
	app(app_)
{
	panel = new wxPanel(this);
	msgs = new tamandua_textctrl(panel, MSGS_CTRL);
	msg = new wxTextCtrl(panel, MSG_CTRL, wxEmptyString, wxPoint(0,0), wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_ENTER);
	sizer = new wxBoxSizer(wxVERTICAL); 

	panel->SetSizer(sizer);
	sizer->Add(msgs, 3, wxALL | wxEXPAND, 10);
	sizer->Add(msg, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 10);
}
