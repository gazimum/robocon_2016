/*
 * controller.cpp
 *
 *  Created on: 2016/07/05
 *      Author: tomoya
 */

#include <map>
#include "controller.hpp"
#include "ini_parser.hpp"
#include "server_shared_data.hpp"

const std::string controller::_my_controller_name = "controller0";

controller::controller() {}

float controller::operator()(std::string key) {
	return _controller_map[key];
}

float controller::get(std::string key) {
	return _controller_map[key];
}

void controller::update() {
	server_shared_data_2016_robocon::_mutex.lock();
	std::map<std::string, int> controller{
		server_shared_data_2016_robocon::_data[network::ports_for_clients.at(_my_controller_name)]
	};
	server_shared_data_2016_robocon::_mutex.unlock();

	ini_parser& config{
		ini_parser::instance()
	};
	float vx = controller[config.key_config("velocity_x")] * _controller_analog_coeff;
	float vy = controller[config.key_config("velocity_y")] * _controller_analog_coeff;
	_controller_map["velocity_x"] = vx;
	_controller_map["velocity_y"] = vy;

	float l = vx * vx + vy * vy;
	if (l > 1.0f) {
		l = sqrt(l);
		vx /= l;
		vy /= l;
	}

	_controller_map["angular_velocity"]  = controller[config.key_config("turn_+")];
	_controller_map["angular_velocity"] -= controller[config.key_config("turn_-")];

	_controller_map["grab"] = controller[config.key_config("grab")];
	_controller_map["lengther"] = controller[config.key_config("lengther")];
	_controller_map["widener"] = controller[config.key_config("widener")];
	_controller_map["height_adjuster"] = controller[config.key_config("height_adjuster")];

}
