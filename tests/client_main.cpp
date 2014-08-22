#include "tamandua.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace tamandua;

void client_connect_callback(tamandua::status st)
{
	if (st == ok)
		std::cout << "\e[1;92mConnected!\e[0m\n";
	else
		std::cout << "\e[1;91mNot connected\e[0m\n";
}

void client_send_callback(tamandua::status st)
{
	if (st != ok)
		std::cout << "!!! Message undelivered! !!!\n";
}

int main(int argc, char ** argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
		return 1;
	}

	boost::asio::io_service io_service;

	client cl(io_service);
	cl.connect(std::string(argv[1]), std::string(argv[2]), client_connect_callback);

	std::thread io_service_thread([&io_service]() { io_service.run(); });
	std::thread display_msg_thread([&cl]() {
		bool running = true;
		while (running)
		{
			if (cl.is_next_message())
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

					}
					std::cout << msg.body << "\e[0m\n";
				}
				else
					std::cout << "<" << author << ">: " << msg.body << "\n";
			}
		}
	});
	std::string msg_body;
	char body[4000];
	while (std::cin.getline(body, 4000))
	{
		message msg;
		std::string body_str(body, strlen(body));
		msg.header.size = body_str.length();
		msg.body = body_str;

		cl.send_message(msg, client_send_callback);
	}

	io_service_thread.join();
	display_msg_thread.join();

	std::cout << "thank you!\n";
	return 0;

}
