/*
 * communication.cpp
 *
 *  Created on: 2016/06/21
 *      Author: tomoya
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "communication.hpp"
//#include "RobotClient.hpp"
//#include <boost/asio.hpp>
#include "server_shared_data.hpp"
#include "network_profile.hpp"

constexpr int port = 100;
constexpr auto ip = "127.0.0.1";

typedef server_shared_data<network::network_node_num> server_shared_data_2016_robocon;

communication::communication() {}

void communication::operator()(){
	//boost::asio::io_service io;
	//RobotClient client(ip, port, 5, "\n", io);
	server_shared_data_2016_robocon&  server_shared_data = server_shared_data_2016_robocon::instance();

	network::ports_for_clients.at("controller0");

	while (true) {
		server_shared_data.get()[5000];
		//server_shared_data.set(server_shared_data.get());
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
