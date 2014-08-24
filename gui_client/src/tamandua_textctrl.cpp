#include "tamandua_textctrl.hpp"

void tamandua_textctrl::add_message(wxString author, wxString msg)
{
	//wxTextAttr author_attr(wxColour(0, 0, 0xff));
	//author_attr.SetFontWeight(wxFONTWEIGHT_BOLD);
	write_lock_.lock();
	long int start = GetInsertionPoint(), end;
	AppendText(author);
	end = GetInsertionPoint();
	wxString colon(wxT(": "), wxMBConvUTF8());
	AppendText(colon);
	AppendText(msg);
	write_lock_.unlock();

	//SetStyle(start, end, author_attr);
}

void tamandua_textctrl::add_info(wxString info)
{
	wxTextAttr attr(wxColour(0xff, 0x66, 0));
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
