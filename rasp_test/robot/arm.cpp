/*
 * arm.cpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#include "arm.hpp"
#include "server_shared_data.hpp"

arm::arm() : _length(float()),
			 _angle(float()),
			 _width(float()) {}

void arm::update() {
	server_shared_data_2016_robocon::_mutex.lock();
	std::map<std::string, int> controller{
		server_shared_data_2016_robocon::_data[network::ports_for_clients.at("controller0")]
	};
	server_shared_data_2016_robocon::_mutex.unlock();

	controller.at("");
}
