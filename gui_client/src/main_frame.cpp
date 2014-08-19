#include "main_frame.hpp"
#include "tamandua.hpp"
#include <string>

main_frame::main_frame(gui_client &app_) :
	wxFrame(0, wxID_ANY, wxT("Tamandua GUI Client"), wxPoint(100,100), wxSize(400,700)),
	app(app_)
{
	panel = new wxPanel(this);
	msgs = new tamandua_textctrl(panel, MSGS_CTRL);
	msg = new wxTextCtrl(panel, MSG_CTRL, wxEmptyString, wxPoint(0,0), wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_ENTER);
	sizer = new wxBoxSizer(wxVERTICAL);
	connect_sizer = new wxBoxSizer(wxHORIZONTAL);
	connect_host = new wxTextCtrl(panel, CON_HOST_TEXT, wxEmptyString, wxPoint(0,0), wxDefaultSize, wxTE_PROCESS_ENTER);
	connect_port = new wxTextCtrl(panel, CON_PORT_TEXT, wxT("5000"), wxPoint(0,0), wxDefaultSize, wxTE_PROCESS_ENTER);
	connect_button = new wxButton(panel, CON_BTN, wxT("Connect"));

	panel->SetSizer(sizer);
	connect_sizer->Add(connect_host,1);
	connect_sizer->Add(connect_port,1);
	connect_sizer->Add(connect_button,1);
	sizer->Add(connect_sizer, 0, wxALL | wxEXPAND, 10);
	sizer->Add(msgs, 3, wxBOTTOM | wxLEFT | wxRIGHT | wxEXPAND, 10);
	sizer->Add(msg, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 10);
}

void main_frame::send_message(wxCommandEvent &event)
{
	wxString data = msg->GetValue();
	msg->Clear();
	msgs->add_message(wxT("ja"), data);
}

void main_frame::connect(wxCommandEvent &event)
{
	std::string host = connect_host->GetValue().ToStdString();
	std::string port = connect_port->GetValue().ToStdString();
	app.get_client()->connect(host, port);
}

BEGIN_EVENT_TABLE(main_frame, wxFrame)
	EVT_TEXT_ENTER(CON_HOST_TEXT, main_frame::connect)
	EVT_TEXT_ENTER(CON_PORT_TEXT, main_frame::connect)
	EVT_TEXT_ENTER(MSG_CTRL, main_frame::send_message)
	EVT_BUTTON(CON_BTN, main_frame::connect)
END_EVENT_TABLE()
