/*
 * controller.cpp
 *
 *  Created on: 2016/07/05
 *      Author: tomoya
 */

#include <ini_parser.hpp>
#include <map>
#include <controller/controller.hpp>
#include <server_shared_data.hpp>
#include <controller/controller_impl.hpp>
#include <controller/basic_controller.hpp>

controller::controller() : _controller_impl(nullptr) {
	_controller_impl = new basic_controller();
}

float controller::operator()(std::string key) {
	return get(key);
}

float controller::get(std::string key) {
	return _controller_impl->get(key);
}

void controller::update() {
	std::string key = "port_for_" + ini_parser::instance().network_profile<std::string>("my_controller_name");
	int my_controllers_port = ini_parser::instance().network_profile<int>(key);

	server_shared_data::_mutex.lock();
	std::map<std::string, int> command {
		server_shared_data::_data[my_controllers_port]
	};
	server_shared_data::_mutex.unlock();

	controller_impl* state = _controller_impl->update(command);
	// 状態遷移の処理
	if (state != _controller_impl) {
		delete _controller_impl;
		_controller_impl = state;
	}
}
