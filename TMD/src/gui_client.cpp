#include <thread>
#include <boost/asio.hpp>
#include "gui_client.hpp"
#include "main_frame.hpp"
#include "tamandua.hpp"
#include "debug_gui.hpp"
#include <functional>
#include <chrono>

bool gui_client::OnInit()
{
	frame = new main_frame();
	frame->Show();
	return true;
}

