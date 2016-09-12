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

void communication::operator()() const {
	try {
		int port = ini_parser::instance().get<int>("network_profile", "server_port");
		std::string server_ip = ini_parser::instance().get<std::string>("network_profile", "server_ip");

		boost::asio::io_service io;
		network::RobotClient client(server_ip, port, 5, "\n", io);
		client.connect();

		size_t network_node_num = ini_parser::instance().get<int>("network_profile", "network_node_num");
		std::string ip_dataset[network_node_num];
		for (size_t i = 0; i < network_node_num; ++i) {
			std::string name {
				ini_parser::instance().get<std::string>("network_profile", "network_device_name_" + std::to_string(i))
			};
			ip_dataset[i] = ini_parser::instance().get<std::string>("network_profile", "ip_for_" + name);
		}

		ini_parser::_mutex.lock();
		ini_parser::_is_usable_for_main_thread = true;
		ini_parser::_mutex.unlock();

		while (!client.is_end()) {
			for (size_t i = 0; i < network_node_num; ++i) {
				server_shared_data::_mutex.lock();
				server_shared_data::_data[ip_dataset[i]] = client.get(ip_dataset[i]);
				server_shared_data::_mutex.unlock();

				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
	}
	catch (const std::string& s) {
		std::cout << s << std::endl;
	}
}
