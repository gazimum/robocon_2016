/*
 * controller.cpp
 *
 *  Created on: 2016/07/05
 *      Author: tomoya
 */

#include <config.hpp>
#include <map>
#include <controller/controller.hpp>
#include <server_shared_data.hpp>
#include <controller/controller_impl.hpp>
#include <controller/flexible_controller.hpp>
#include <controller/simple_controller.hpp>
#include <pid/pid_manager.hpp>
#include <lpf/lpf_manager.hpp>

#include <iostream>

controller::controller() : _controller_impl(nullptr) {
	_controller_impl = new simple_controller();
	_time = std::chrono::system_clock::now();
}

controller::~controller() {
	delete _controller_impl;
}

float controller::operator()(std::string key) {
	return get(key);
}

float controller::get(std::string key) {
	return _command[key];
}

void controller::update() {
	std::string value_key = "ip_for_" + config::instance().get<std::string>("network_profile", "my_controller_name");
	std::string ip = config::instance().get<std::string>("network_profile", value_key);

	server_shared_data::_mutex.lock();
	server_shared_data::container_type controller_state {
		server_shared_data::_dataset
	};
	server_shared_data::_mutex.unlock();

	controller_impl* state = _controller_impl->update(controller_state);
	_command = _controller_impl->get_command();
	// 状態遷移の処理
	auto now = std::chrono::system_clock::now();
	float t_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - _time).count();
	if (t_ms < config::instance().get<float>("setting", "controller_sequence_wait_time_ms")) {
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

	// 設定ファイルをリロードしたときに設定ファイルから値を設定しなおす
	if (_command["reload_ini_file"] > 0.0f) {
		for (const auto& i : _reload_ini_file_value_function_dataset) {
			i();
		}
		pid_manager<float>::instance().config();
		lpf_manager<float>::instance().config();
		_controller_impl->reload_config_value();
	}
}

void controller::add_reload_ini_file_value_function(std::function<void()> f) {
	_reload_ini_file_value_function_dataset.push_back(f);
}
