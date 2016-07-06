/*
 * controller.cpp
 *
 *  Created on: 2016/07/05
 *      Author: tomoya
 */

#include "controller.hpp"
#include "ini_parser.hpp"
#include "server_shared_data.hpp"

const std::string controller::_controller_name = "controller0";

void controller::operator()() {
	get(name);
}

void controller::update() {
	server_shared_data_2016_robocon::_mutex.lock();
	auto c{
		server_shared_data_2016_robocon::_data[network::ports_for_clients.at("controller0")]
	};
	server_shared_data_2016_robocon::_mutex.unlock();

	auto config =  ini_parser::instance();
	_velocity_x = c[config.key_config("move_x")] * _controller_analog_coeff;
	_velocity_y = c[config.key_config("move_y")] * _controller_analog_coeff;


}

float controller::get_velocity_x() const {
	return _velocity_x;
}

float controller::get_velocity_y() const {
	return _velocity_y;
}

float controller::get_angular_velocity() const {
	return _angular_velocity;
}


