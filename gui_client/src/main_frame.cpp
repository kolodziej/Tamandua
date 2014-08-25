#include "main_frame.hpp"
#include "wx/app.h"
#include "tamandua.hpp"
#include "tamandua_box.hpp"
#include "debug_gui.hpp"
#include <string>

wxDECLARE_APP(gui_client);

main_frame::main_frame() :
	wxFrame(0, wxID_ANY, wxT("Tamandua GUI Client"), wxPoint(100,100), wxSize(400,700))
{
	panel = new wxPanel(this);
	msgs = new tamandua_textctrl(panel, MSGS_CTRL);
	msg = new wxTextCtrl(panel, MSG_CTRL, wxEmptyString, wxPoint(0,0), wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_ENTER);
	sizer = new wxBoxSizer(wxVERTICAL);
	connect_sizer = new wxBoxSizer(wxHORIZONTAL);
	connect_host = new wxTextCtrl(panel, CON_HOST_TEXT, wxT("localhost"), wxPoint(0,0), wxDefaultSize, wxTE_PROCESS_ENTER);
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
	tamandua::message msg(tamandua::message_type::standard_message, std::string(data.utf8_str()));
	tb->client.send_message(msg, [this](tamandua::status s) {
		if (s == tamandua::status::ok)
			message_sent_();
		else
			message_undelivered_();
	});
}

void main_frame::connect(wxCommandEvent &event)
{
	std::string host(connect_host->GetValue().utf8_str());
	std::string port(connect_port->GetValue().utf8_str());

	tb = new tamandua_box;
	tb->client.connect(host, port, [this](tamandua::status st)
	{
		if (st == tamandua::status::ok)
			connecting_succeeded_();
		else
			connecting_failed_();
	});
	tb->io_service_thread = std::thread([this]() {
		tb->io_service.run();
	});
	tb->reader_thread = std::thread([this]() {
		tamandua::client &cl = tb->client;
		bool local_running = true;
		do {
			if (cl.is_next_message())
			{
				auto msg_pair = cl.get_next_message();
				wxString author = wxString::FromUTF8(msg_pair.first.data());
				wxString msg_body = wxString::FromUTF8(msg_pair.second.body.data());
				tamandua::message &msg = msg_pair.second;
				switch (msg.header.type)
				{
					case tamandua::message_type::info_message:
						Debug("info: ", msg_body);
						wxTheApp->GetTopWindow()->GetEventHandler()->CallAfter(std::bind(&tamandua_textctrl::add_info, msgs, msg_body));
						break;

					case tamandua::message_type::error_message:
						Debug("error: ", msg_body);
						wxTheApp->GetTopWindow()->GetEventHandler()->CallAfter(std::bind(&tamandua_textctrl::add_error, msgs, msg_body));
						break;

					case tamandua::message_type::warning_message:
						Debug("warning: ", msg_body);
						wxTheApp->GetTopWindow()->GetEventHandler()->CallAfter(std::bind(&tamandua_textctrl::add_warning, msgs, msg_body));
						break;

					case tamandua::message_type::private_message:
						Debug("@", author, ": ", msg_body);
						wxTheApp->GetTopWindow()->GetEventHandler()->CallAfter(std::bind(&tamandua_textctrl::add_private_message, msgs, author, msg_body));
						break;
					
					case tamandua::message_type::standard_message:
						Debug(author, ": ", msg_body);
						wxTheApp->GetTopWindow()->GetEventHandler()->CallAfter(std::bind(&tamandua_textctrl::add_message, msgs, author, msg_body));
						break;
				}
			}
			std::this_thread::yield();
			std::lock_guard<std::mutex> lock(tb->running_lock);
			local_running = tb->running;
		} while (local_running);
	});

	connect_button->Unbind(wxEVT_BUTTON, &main_frame::connect, this);
	connect_button->Bind(wxEVT_BUTTON, &main_frame::disconnect, this);
	connect_button->SetLabel(wxT("Disconnect"));
}

void main_frame::disconnect(wxCommandEvent &event)
{
	msgs->add_info("Disconnecting...");
	


	connect_button->Unbind(wxEVT_BUTTON, &main_frame::disconnect, this);
	connect_button->Bind(wxEVT_BUTTON, &main_frame::connect, this);
	connect_button->SetLabel(wxString(wxT("Connect")));
}

void main_frame::connecting_succeeded_()
{
	connected = true;
	connect_button->SetLabel(wxString(wxT("Disconnect")));
	Debug("Connected to server!");
	wxString info(wxT("Connected to server!"), wxMBConvUTF8());
	msgs->add_info(info);
}

void main_frame::connecting_failed_()
{
	Debug("Connecting failed!");
	wxString error(wxT("Connecting failed!"), wxMBConvUTF8());
	msgs->add_error(error);
}

void main_frame::message_sent_()
{
	Debug("Message sent!");
}

void main_frame::message_undelivered_()
{
	wxString error(wxT("Message undelivered!"), wxMBConvUTF8());
	msgs->add_error(error);
}

BEGIN_EVENT_TABLE(main_frame, wxFrame)
//	EVT_TEXT_ENTER(CON_HOST_TEXT, main_frame::connect)
//	EVT_TEXT_ENTER(CON_PORT_TEXT, main_frame::connect)
	EVT_TEXT_ENTER(MSG_CTRL, main_frame::send_message)
	EVT_BUTTON(CON_BTN, main_frame::connect)
END_EVENT_TABLE()
