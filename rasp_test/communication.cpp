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
#include <communication.hpp>
#include <ini_parser.hpp>
#include <network/RobotClient.hpp>
#include <server_shared_data.hpp>
#include <iostream>
#include <map>
#include <string>
#include <ini_parser.hpp>

communication::communication() {}

void communication::operator()() {
	boost::asio::io_service io;
	network::RobotClient client(ini_parser::instance().network_profile<std::string>("server_ip_address"),
								ini_parser::instance().network_profile<int>("my_port"),
								40, "\n", io);
	client.connect();

	while (true) {
		/*
		for (size_t i = 0; i < ini_parser::instance().network_profile<int>("network_node_num"); ++i) {
			std::string name{
				ini_parser::instance().network_profile<std::string>("network_device_name" + std::to_string(i))
			};
			int port = ini_parser::instance().network_profile<int>("port_for_" + name);

			server_shared_data::_mutex.lock();
			server_shared_data::_data[port] = client.get(port);
			std::this_thread::sleep_for(std::chrono::milliseconds(80));
			server_shared_data::_mutex.unlock();
		}
		*/

		std::map<std::string, int> d;
		server_shared_data::_mutex.lock();
		d = server_shared_data::_data[5000] = client.get(5000);
		server_shared_data::_mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(40));

		/*
		for (const auto& i : d) {
			std::cout << i.first << "," << i.second << " ";
		}
		std::cout << std::endl;*/
	}
}
