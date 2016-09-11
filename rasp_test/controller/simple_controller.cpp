/*
 * simplecontroller.cpp
 *
 *  Created on: 2016/08/17
 *      Author: u
 */

#include <string>
#include <controller/simple_controller.hpp>
#include <controller/basic_controller.hpp>
#include <controller/flexible_controller.hpp>
#include <ini_parser.hpp>
#include <pid/pid_manager.hpp>

std::string simple_controller::_state_name = "very_low";
state_machine simple_controller::_state_machine("height_low");

simple_controller::simple_controller() {
	_state_machine.add_state("release", 		std::bind(&simple_controller::release, 		 this));
	_state_machine.add_state("height_low",		std::bind(&simple_controller::height_low, 	 this));
	_state_machine.add_state("grab", 			std::bind(&simple_controller::grab, 		 this));
	_state_machine.add_state("height_adjust", 	std::bind(&simple_controller::height_adjust, this));

	reload_ini_file_value();
	_time = std::chrono::system_clock::now();
}

simple_controller::~simple_controller() {}

controller_impl* simple_controller::update() {
	_state_machine.update();
	update_state_by_state_name();
	update_lock();
	update_movement();
	return update_sequence();
}

void simple_controller::update_pid_index() {
	pid_manager<float>::instance().set_index(_state_index_dataset.at(_state_name));
	pid_manager<float>::instance().config();
}

controller_impl* simple_controller::update_sequence() {
	if (is_key_pushed("controller_switch_1") && is_key_rise("controller_switch_2")) {
		if (is_key_pushed("controller_switch_3")) {
			return new flexible_controller;
		}
		return new basic_controller;
	}
	return this;
}

void simple_controller::update_lock() {
	if (is_key_rise("lock")) {
		_is_lock_enable = !_is_lock_enable;
	}
}

std::string simple_controller::release() {
	_is_grab_enable = false;

	auto elapsed = std::chrono::system_clock::now() - _time;
	float elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
	float wait_time_ms = ini_parser::instance().get<float>("setting", "release_wait_time_ms");
	bool is_transition = (elapsed_ms > wait_time_ms);
	is_transition = is_transition && is_key_rise("grab");
	if (is_transition) {
		return "height_low";
	}

	return "release";
}

std::string simple_controller::height_low() {
	_state_name = "very_low";

	if (is_key_rise("grab")){
		_time = std::chrono::system_clock::now();
		return "grab";
	}

	return "height_low";
}

std::string simple_controller::grab() {
	_is_grab_enable = true;

	auto elapsed = std::chrono::system_clock::now() - _time;
	float elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
	float wait_time_ms = ini_parser::instance().get<float>("setting", "release_wait_time_ms");
	if (elapsed_ms > wait_time_ms) {
		return "height_adjust";
	}
	return "grab";
}

std::string simple_controller::height_adjust() {
	update_state_name();

	if (is_key_rise("grab")){
		_time = std::chrono::system_clock::now();
		return "release";
	}

	return "height_adjust";
}

void simple_controller::update_state_name() {
	int index = controller_impl::read_arm_ability_position_index();
	if (index < 0) {
		return;
	}
	if (is_key_pushed("height_high")){
		index += ini_parser::instance().get<int>("key_config", "height_low_key_num");
	}
	int arm_state_num = ini_parser::instance().get<int>("arm_state", "arm_state_num");
	if (index >= arm_state_num) {
		index = arm_state_num - 1;
	}
	std::string value_key {
		"state" + std::to_string(index) + "_name"
	};
	_state_name = ini_parser::instance().get<std::string>("arm_state", value_key);
}

void simple_controller::update_state_by_state_name() {
	for (const auto& i : _arm_ability_name_dataset) {
		std::string value_key {
			"state" + std::to_string(_state_index_dataset[_state_name]) + "_" + i + "_index"
		};
		int index = ini_parser::instance().get<int>("arm_state", value_key);
		_arm_ability_position_index_dataset[i] = index;
		_command[i] = ini_parser::instance().get<float>(i, "position" + std::to_string(index));
	}
}

void simple_controller::reload_ini_file_value() {
	for (size_t i = 0; i < ini_parser::instance().get<int>("arm_state", "arm_state_num"); ++i) {
		std::string name = ini_parser::instance().get<std::string>("arm_state", "state" + std::to_string(i) + "_name");
		_state_index_dataset[name] = i;
	}
}
