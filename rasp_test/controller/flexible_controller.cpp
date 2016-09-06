/*
 * flexiblecontroller.cpp
 *
 *  Created on: 2016/09/06
 *      Author: u
 */

#include <controller/simple_controller.hpp>
#include <controller/basic_controller.hpp>
#include <controller/flexible_controller.hpp>
#include <ini_parser.hpp>
#include <string>

std::string flexible_controller::_state_name = "very_low";
bool flexible_controller::_is_lock_enable = false;
bool flexible_controller::_is_grab_enable = false;

flexible_controller::flexible_controller() {
	for (size_t i = 0; i < ini_parser::instance().get<int>("arm_state", "arm_state_num"); ++i) {
		std::string name = ini_parser::instance().get<std::string>("arm_state", "state" + std::to_string(i) + "_name");
		_state_index_dataset[name] = i;
	}
}

flexible_controller::~flexible_controller() {}

controller_impl* flexible_controller::update() {
	update_movement();
	update_angle_base();
	update_grab();
	update_lock();

	update_state_name();
	update_state_by_state_name();

	return update_sequence();
}

void flexible_controller::update_state_name() {
	int index = controller_impl::read_arm_ability_position_index();
	if (index < 0) {
		return;
	}
	std::string high_key {
		ini_parser::instance().get<std::string>("key_config", "height_high")
	};
	if (is_key_pushed(high_key)){
		index += ini_parser::instance().get<int>("key_config", "height_low_key_num");
	}
	int arm_state_num = ini_parser::instance().get<int>("arm_state", "arm_state_num");
	if (index >= arm_state_num) {
		index = arm_state_num - 1;
	}
	_state_name = ini_parser::instance().get<std::string>("arm_state", "state" + std::to_string(index) + "_name");
}

void flexible_controller::update_grab() {
	std::string grab_key {
		ini_parser::instance().get<std::string>("key_config", "grab")
	};

	if (is_key_rise(grab_key)) {
		_is_grab_enable = !_is_grab_enable;
	}

	if (_is_grab_enable) {
		_command["width"] = 1.0f;
	} else {
		_command["width"] = -1.0f;
	}
}

controller_impl* flexible_controller::update_sequence() {
	std::string key[] {
		ini_parser::instance().get<std::string>("key_config", "controller_switch_1"),
		ini_parser::instance().get<std::string>("key_config", "controller_switch_2"),
		ini_parser::instance().get<std::string>("key_config", "controller_switch_3")
	};
	if (is_key_pushed(key[0]) && is_key_rise(key[1])) {
		if (is_key_pushed(key[2])) {
			return new simple_controller;
		}
		return new basic_controller;
	}
	return this;
}

void flexible_controller::update_lock() {
	std::string key {
		ini_parser::instance().get<std::string>("key_config", "lock")
	};
	if (is_key_rise(key)) {
		_is_lock_enable = !_is_lock_enable;
	}

	if (_is_lock_enable) {
		_command["lock"] = 1.0f;
	} else {
		_command["lock"] = -1.0f;
	}
}

void flexible_controller::update_movement() {
	ini_parser& ini {
		ini_parser::instance()
	};

	// 平行移動
	float vx = _normalized_controller_state[ini.get<std::string>("key_config", "velocity_x")];
	float vy = _normalized_controller_state[ini.get<std::string>("key_config", "velocity_y")];

	float l = vx * vx + vy * vy;
	if (l > 1.0f) {
		l = sqrt(l);
		vx /= l;
		vy /= l;
	}
	_command["velocity_x"] = vx;
	_command["velocity_y"] = vy;

	// 旋回
	_command["angular_velocity"]  = _normalized_controller_state[ini.get<std::string>("key_config", "turn_+")];
	_command["angular_velocity"] -= _normalized_controller_state[ini.get<std::string>("key_config", "turn_-")];
}

void flexible_controller::update_angle_base() {
	_command["angle_base"] = _command["height"];
	_command["angle_left"]  = _command["angle"];
	_command["angle_right"] = _command["angle"];
}

void flexible_controller::update_state_by_state_name() {
	for (const auto& i : _arm_ability_name_dataset) {
		std::string key {
			"state" + std::to_string(_state_index_dataset[_state_name]) + "_" + i + "_index"
		};
		int index = ini_parser::instance().get<int>("arm_state", key);
		_arm_ability_position_index_dataset[i] = index;
		_command[i] = ini_parser::instance().get<float>("arm_" + i, "position" + std::to_string(index));
	}
}

