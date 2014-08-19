#include "gui_client.hpp"

bool gui_client::OnInit()
{
	frame = new main_frame(*this);
	frame->Show();
	return true;
}
