#include "tamandua_textctrl.hpp"

void tamandua_textctrl::add_message(wxString author, wxString msg)
{
	wxTextAttr author_attr(wxColour(0, 0, 0xff));
	author_attr.SetFontWeight(wxFONTWEIGHT_BOLD);
	wxString input(author + wxT(": ") + msg);
	write_lock_.lock();
	long int start = GetInsertionPoint(), end;
	AppendText(author);
	end = GetInsertionPoint();
	AppendText(": ");
	AppendText(msg);
	AppendText("\n");
	write_lock_.unlock();

	SetStyle(start, end, author_attr);
}

void tamandua_textctrl::add_info(wxString info)
{
	wxTextAttr attr(wxColour(0x00, 0x99, 0));
	attr.SetFontWeight(wxFONTWEIGHT_BOLD);
	info += wxString("\n", wxMBConvUTF8());

	write_lock_.lock();
	long int start = GetInsertionPoint(), end;
	AppendText(info);
	end = GetInsertionPoint();
	write_lock_.unlock();

	SetStyle(start, end, attr);
}

void tamandua_textctrl::add_error(wxString error)
{
	wxTextAttr attr(wxColour(0xcc, 0, 0));
	attr.SetFontWeight(wxFONTWEIGHT_BOLD);
	error += wxString("\n", wxMBConvUTF8());

	write_lock_.lock();
	long int start = GetInsertionPoint(), end;
	AppendText(error);
	end = GetInsertionPoint();
	write_lock_.unlock();

	SetStyle(start, end, attr);
}
