/*
 * communication.cpp
 *
 *  Created on: 2016/06/21
 *      Author: tomoya
 */

#include <iostream>
#include <thread>
#include <chrono>
#include "communication.hpp"
//#include "RobotClient.hpp"
//#include <boost/asio.hpp>
#include "server_shared_data.hpp"

constexpr int port = 5000;
constexpr auto ip = "127.0.0.1";
constexpr size_t server_shared_data_num = 7;

typedef server_shared_data<server_shared_data_num> server_shared_data_2016_robocon;

communication::communication() {}

void communication::operator()(){
	//boost::asio::io_service io;
	//RobotClient client(ip, port, 5, "\n", io);


	while (true) {
		server_shared_data_2016_robocon::instance().get();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
