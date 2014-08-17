#include "tamandua.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace tamandua;

int main(int argc, char ** argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
		return 1;
	}

	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);
	tcp::resolver::iterator endpoint_it = resolver.resolve({ argv[1], argv[2] });

	client cl(io_service, endpoint_it);

	std::thread io_service_thread([&io_service]() { io_service.run(); });
	std::thread display_msg_thread([&cl]() {
		if (cl.is_next_message())
		{
			auto msg = cl.get_next_message();
			std::cout << "<" << msg.first << ">: " << msg.second << "\n";
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

		cl.send_message(msg);
	}

	io_service_thread.join();
	display_msg_thread.join();

	std::cout << "thank you!\n";
	return 0;

}
