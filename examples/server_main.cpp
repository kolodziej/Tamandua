#include "tamandua_local.hpp"
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include "../headers/modules/base_user_module.hpp"

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
	boost::asio::ssl::context context(boost::asio::ssl::context::sslv23);
	context.set_options(
		boost::asio::ssl::context::default_workarounds |
		boost::asio::ssl::context::no_sslv2 |
		boost::asio::ssl::context::single_dh_use);
	context.use_certificate_chain_file("ssl/server.crt");
	context.use_private_key_file("ssl/server.key", boost::asio::ssl::context::pem);
	context.use_tmp_dh_file("ssl/dh512.pem");

	tcp::endpoint endpoint(tcp::v4(), stoi(port));
	logger log(std::cerr);
	command_interpreter interpreter('/');

	server_config cfg;
	cfg.server_name = "Testing tamandua server";
	cfg.max_participants_number = 2000;
	cfg.main_room_name = "HallOfFame";
	cfg.root_password = "testpass";
	server svr(io_service, endpoint, log, interpreter, context);
	base_user_module user_module(svr, interpreter);
	svr.register_module(user_module);
	svr.start_server(cfg);
	io_service.run();
	return 0;
}
