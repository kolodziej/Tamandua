#include "tamandua.hpp"
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

using namespace tamandua;

int main(int argc, char ** argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage " << argv[0] << " <port>\n";
		return 1;
	}
	std::string port(argv[1]);
	boost::asio::io_service io_service;
	tcp::endpoint endpoint(tcp::v4(), stoi(port));
	logger log(std::cerr);
	server svr(io_service, endpoint, log);
	io_service.run();
	return 0;
}
