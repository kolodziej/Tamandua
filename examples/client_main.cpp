#include "tamandua_local.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <utility>
#include <functional>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace tamandua;

void connecting_succeeded_callback(status st)
{
	TamanduaDebug("Connected succesfully!");
}

void connecting_failed_callback(status st)
{
	TamanduaDebug("Connecting failed");
}

void message_sent_callback(status st)
{
	TamanduaDebug("Message sent");
}

int main(int argc, char ** argv)
{
	std::cout << "Testing Tamandua client.\nUsing Tamandua Library " << tamandua::get_version_str() << " (" << tamandua::get_release_type() << ")\n";
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
		return 1;
	}

	boost::asio::ssl::context context(boost::asio::ssl::context::sslv23);
	context.load_verify_file("ssl/server.pem");
	context.set_verify_mode(boost::asio::ssl::verify_peer);
	context.set_verify_callback([&](bool pv, boost::asio::ssl::verify_context &ctx)
	{
		TamanduaDebug("Verification: ", pv);
		return true;
	});

	client cl(context);
	cl.add_event_handler(client::connecting_succeeded, std::bind(&connecting_succeeded_callback, std::placeholders::_1));
	cl.add_event_handler(client::connecting_failed, std::bind(&connecting_failed_callback, std::placeholders::_1));
	cl.add_event_handler(client::message_sent, std::bind(&message_sent_callback, std::placeholders::_1));
	cl.connect(std::string(argv[1]), std::string(argv[2]));

	std::thread display_msg_thread([&cl]() {
		bool running = true;
		while (running)
		{
			auto msg_pair = cl.get_next_message();
			std::string &author = msg_pair.first;
			message &msg = msg_pair.second;
			if (msg.header.type != message_type::standard_message)
			{
				switch (msg.header.type)
				{
					case message_type::info_message:
						std::cout << "\e[1;33m";
						break;

					case message_type::error_message:
						std::cout << "\e[1;91m";
						break;

					case message_type::group_enter_message:
						std::cout << "entering new group: " << msg.header.group;
						break;

					default:
						break;
				}
				std::cout << msg.body << "\e[0m\n";
			}
			else
			{
				char time_buff[30];
				tm *ptr = localtime(reinterpret_cast<const time_t*>(&msg.header.utc_time));
				strftime(time_buff, 30, "%c", ptr);
				std::cout << "[" << time_buff << "] <" << author << ">: " << msg.body << "\n";
			}
		}
	});
	std::string msg_body;
	char body[4000];
	while (std::cin.getline(body, 4000))
	{
		message msg;
		std::string body_str(body, strlen(body));
		cl.send_message(body_str);
	}

	display_msg_thread.join();

	std::cout << "thank you!\n";
	return 0;

}
