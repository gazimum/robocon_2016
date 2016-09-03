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
	try {
		int port = ini_parser::instance().get<int>("network_profile", "server_port");
		std::string server_ip = ini_parser::instance().get<std::string>("network_profile", "server_ip");

		boost::asio::io_service io;
		network::RobotClient client(server_ip, port, 5, "\n", io);
		client.connect();

		while (!client.is_end()) {
			for (size_t i = 0; i < ini_parser::instance().get<int>("network_profile", "network_node_num"); ++i) {
				std::string name{
					ini_parser::instance().get<std::string>("network_profile", "network_device_name" + std::to_string(i))
				};
				std::string ip = ini_parser::instance().get<std::string>("network_profile", "ip_for_" + name);

				server_shared_data::_mutex.lock();
				server_shared_data::_data[ip] = client.get(ip);
				server_shared_data::_mutex.unlock();

				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
	}
	catch (const char* s) {
		std::cout << s << std::endl;
	}
	catch (std::string& s) {
		std::cout << s << std::endl;
	}
}
