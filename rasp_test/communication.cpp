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
#include <boost/asio.hpp>
#include "communication.hpp"
#include "./network/RobotClient.hpp"
#include "server_shared_data.hpp"
#include "./network/network_profile.hpp"

communication::communication() {}

void communication::operator()() {
	std::cout << "start communication thread" << std::endl;

	boost::asio::io_service io;
	network::RobotClient client(network::server_ip_address, network::my_port, 5, "\n", io);
	client.connect();

	server_shared_data_2016_robocon&  server_shared_data = server_shared_data_2016_robocon::instance();

	while (true) {
		std::cout << "communication thread" << std::endl;

		client.set(server_shared_data.get()[network::my_port]);
		for (uint32_t i = 0; i < network::network_node_num; ++i) {
			client.get(network::ports_for_clients[network::]);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
