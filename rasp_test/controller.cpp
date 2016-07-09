/*
 * controller.cpp
 *
 *  Created on: 2016/07/05
 *      Author: tomoya
 */

#include <ini_parser.hpp>
#include <map>
#include "controller.hpp"
#include "server_shared_data.hpp"

controller::controller() {}

float controller::operator()(std::string key) {
	return _controller_map[key];
}

float controller::get(std::string key) {
	return _controller_map[key];
}

void controller::update() {
	std::string key = "port_for_" + ini_parser::instance().network_profile<std::string>("my_controller_name");
	int my_controllers_port = ini_parser::instance().network_profile<int>(key);

	server_shared_data::_mutex.lock();
	std::map<std::string, int> controller{
		server_shared_data::_data[my_controllers_port]
	};
	server_shared_data::_mutex.unlock();

	ini_parser& config{
		ini_parser::instance()
	};
	float vx =  controller[config.key_config<std::string>("velocity_x")] * _controller_analog_coeff;
	float vy = -controller[config.key_config<std::string>("velocity_y")] * _controller_analog_coeff;
	_controller_map["velocity_x"] = vx;
	_controller_map["velocity_y"] = vy;

	float l = vx * vx + vy * vy;
	if (l > 1.0f) {
		l = sqrt(l);
		vx /= l;
		vy /= l;
	}

	_controller_map["angular_velocity"]  = controller[config.key_config<std::string>("turn_+")];
	_controller_map["angular_velocity"] -= controller[config.key_config<std::string>("turn_-")];

	_controller_map["grab"] = controller[config.key_config<std::string>("grab")];
	_controller_map["lengther"] = controller[config.key_config<std::string>("lengther")];
	_controller_map["widener"] = controller[config.key_config<std::string>("widener")];
	_controller_map["angle_adjuster"] = controller[config.key_config<std::string>("angle_adjuster")];

}
