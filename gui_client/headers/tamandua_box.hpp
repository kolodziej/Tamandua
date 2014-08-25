#ifndef TAMANDUA_BOX_HPP
#define TAMANDUA_BOX_HPP
#include "tamandua.hpp"
#include <thread>
#include <mutex>
#include <boost/asio.hpp>

struct tamandua_box
{
	boost::asio::io_service io_service;
	tamandua::client client;
	std::thread reader_thread, io_service_thread;
	std::mutex running_lock;
	bool running;

	tamandua_box() :
		client(io_service),
		running(true)
	{}
};

#endif
