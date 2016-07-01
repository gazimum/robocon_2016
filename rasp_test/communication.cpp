/*
 * communication.cpp
 *
 *  Created on: 2016/06/21
 *      Author: tomoya
 */

// todo : delete debug
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
	boost::asio::io_service io;
	network::RobotClient client(network::server_ip_address, network::my_port, 5, "\n", io);
	client.connect();

	server_shared_data_2016_robocon&  server_shared_data = server_shared_data_2016_robocon::instance();

	while (true) {
		//std::cout << "communication thread" << std::endl;

		//client.set(server_shared_data.get()[network::my_port]);

		/*
		for (const auto& port : network::ports_for_clients) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			//server_shared_data.get()[port.second] = client.get(port.second);
		}
		*/

		server_shared_data.get()[5000] = client.get(5000);
		server_shared_data.get()[5000]["stkLY"] *= -1;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		//server_shared_data.get()[5001] = client.get(5001);

		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
