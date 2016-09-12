/*
 * flexiblecontroller.cpp
 *
 *  Created on: 2016/09/06
 *      Author: u
 */

#include <string>
#include <controller/controller.hpp>
#include <controller/simple_controller.hpp>
#include <controller/basic_controller.hpp>
#include <controller/flexible_controller.hpp>
#include <ini_parser.hpp>
#include <pid/pid_manager.hpp>
#include <lpf/lpf_manager.hpp>
#include <iostream>

std::string flexible_controller::_state_name = "very_low";

flexible_controller::flexible_controller() {
	reload_ini_file_value();
}

flexible_controller::~flexible_controller() {}

controller_impl* flexible_controller::update() {
	update_movement();
	update_grab();
	update_lock();

	update_state_name();
	update_state_by_state_name();
	return update_sequence();
}

void flexible_controller::update_pid_index() {
	pid_manager<float>::instance().set_index(_state_index_dataset.at(_state_name));
	pid_manager<float>::instance().config();
}

void flexible_controller::update_lpf_index() {
	lpf_manager<float>::instance().set_index(_state_index_dataset.at(_state_name));
	lpf_manager<float>::instance().config();
}

void flexible_controller::update_state_name() {
	int index = controller_impl::read_arm_ability_position_index();
	if (index < 0) {
		return;
	}
	if (is_key_pushed("arm_index_mode_2_switch")){
		index += ini_parser::instance().get<int>("key_config", "arm_index_mode_1_key_num");
	}
	int arm_state_num = ini_parser::instance().get<int>("arm_state", "arm_state_num");
	if (index >= arm_state_num) {
		index = arm_state_num - 1;
	}
	std::string value_key {
		"state_" + std::to_string(index) + "_name"
	};
	_state_name = ini_parser::instance().get<std::string>("arm_state", value_key);
}

void flexible_controller::update_grab() {
	if (is_key_rise("grab")) {
		_is_grab_enable = !_is_grab_enable;
	}
}

controller_impl* flexible_controller::update_sequence() {
	if (is_key_pushed("controller_switch_1") && is_key_rise("controller_switch_2")) {
		if (is_key_pushed("controller_switch_3")) {
			return new simple_controller;
		}
		return new basic_controller;
	}
	return this;
}

void flexible_controller::update_lock() {
	if (is_key_rise("lock")) {
		_is_lock_enable = !_is_lock_enable;
	}
}

void flexible_controller::update_state_by_state_name() {
	for (const auto& i : _arm_ability_name_dataset) {
		std::string value_key {
			"state_" + std::to_string(_state_index_dataset[_state_name]) + "_" + i + "_index"
		};
		int index = ini_parser::instance().get<int>("arm_state", value_key);
		_arm_ability_position_index_dataset[i] = index;
		_command[i] = ini_parser::instance().get<float>(i, "position_" + std::to_string(index));
	}
}

void flexible_controller::reload_ini_file_value() {
	size_t arm_state_num = ini_parser::instance().get<int>("arm_state", "arm_state_num");
	for (size_t i = 0; i < arm_state_num; ++i) {
		std::string name = ini_parser::instance().get<std::string>("arm_state", "state_" + std::to_string(i) + "_name");
		_state_index_dataset[name] = i;
	}
}

