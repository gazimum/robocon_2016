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
#include <config.hpp>
#include <config.hpp>
#include <network/RobotClient.hpp>
#include <server_shared_data.hpp>
#include <iostream>
#include <map>
#include <string>

communication::communication() {}

void communication::operator()() const {
	try {
		int port = config::instance().get<int>("network_profile", "server_port");
		std::string server_ip = config::instance().get<std::string>("network_profile", "server_ip");

		boost::asio::io_service io;
		network::RobotClient client(server_ip, port, 5, "\n", io);

		// 定数を設定ファイルから読みだしておく
		size_t network_device_num = config::instance().get<int>("network_profile", "network_device_num");
		std::string name_dataset[network_device_num];
		std::string ip_dataset[network_device_num];
		for (size_t i = 0; i < network_device_num; ++i) {
			std::string name {
				config::instance().get<std::string>("network_profile", "network_device_name_" + std::to_string(i))
			};
			name_dataset[i] = name;
			ip_dataset[i] = config::instance().get<std::string>("network_profile", "ip_for_" + name);
		}

		config::_mutex.lock();
		config::_is_usable_for_main_thread = true;
		config::_mutex.unlock();

		client.connect();

		while (!client.is_end()) {
			for (size_t i = 0; i < network_device_num; ++i) {
				server_shared_data::_mutex.lock();
				server_shared_data::_dataset[name_dataset[i]] = client.get(ip_dataset[i]);
				server_shared_data::_mutex.unlock();

				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
	}
	catch (const std::string& s) {
		std::cout << s << std::endl;
	}
}
