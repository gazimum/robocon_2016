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
	_time = std::chrono::system_clock::now();
}

controller::~controller() {
	delete _controller_impl;
}

float controller::operator()(std::string key) {
	return get(key);
}

float controller::get(std::string key) {
	return _controller_impl->get(key);
}

void controller::update() {
	std::string key = "ip_for_" + ini_parser::instance().get<std::string>("network_profile", "my_controller_name");
	std::string ip = ini_parser::instance().get<std::string>("network_profile", key);

	server_shared_data::_mutex.lock();
	std::map<std::string, int> command {
		server_shared_data::_data[ip]
	};
	server_shared_data::_mutex.unlock();

	controller_impl* state = _controller_impl->update(command);
	// 状態遷移の処理
	auto now = std::chrono::system_clock::now();
	float t = std::chrono::duration_cast<std::chrono::milliseconds>(now - _time).count();
	if (t < ini_parser::instance().get<float>("setting", "controller_sequence_wait_time_ms")) {
		if (state != _controller_impl) {
			delete state;
		}
		return;
	}

	if (state != _controller_impl) {
		delete _controller_impl;
		_controller_impl = state;
		_time = now;
	}
}

void controller::add_reload_ini_file_value_function(std::function<void()> f) {
	_controller_impl->add_reload_ini_file_value_function(f);
}
