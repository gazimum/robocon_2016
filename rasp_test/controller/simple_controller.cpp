/*
 * simplecontroller.cpp
 *
 *  Created on: 2016/08/17
 *      Author: u
 */

#include <controller/simple_controller.hpp>
#include <controller/basic_controller.hpp>
#include <ini_parser.hpp>
#include <string>

simple_controller::simple_controller() : _state_machine("release"),
											   _state_name("very_low") {
	_state_machine.add_state("release", 		std::bind(&simple_controller::release, 		 this));
	_state_machine.add_state("height_low",		std::bind(&simple_controller::height_low, 	 this));
	_state_machine.add_state("grab", 			std::bind(&simple_controller::grab, 		 this));
	_state_machine.add_state("height_adjust", 	std::bind(&simple_controller::height_adjust, this));

	for (size_t i = 0; i < ini_parser::instance().get<int>("arm_state", "arm_state_num"); ++i) {
		std::string name = ini_parser::instance().get<std::string>("arm_state", "state" + std::to_string(i) + "_name");
		_state_index_list[name] = i;
	}

	_time = std::chrono::system_clock::now();
}

simple_controller::~simple_controller() {}

void simple_controller::update_ini_parser() {
	std::string key {
		ini_parser::instance().get<std::string>("key_config", "reload_ini_file")
	};
	if (is_key_rise(key)) {
		ini_parser::instance().read();
		_command["reload_ini_file"] = 1.0f;
	} else {
		_command["reload_ini_file"] = -1.0f;
	}
}

controller_impl* simple_controller::update() {
	_state_machine.update();
	update_state_by_state_name();
	update_lock();
	update_movement();
	update_angle_base();
	update_ini_parser();
	return update_sequence();
}

controller_impl* simple_controller::update_sequence() {
	std::string key[] {
		ini_parser::instance().get<std::string>("key_config", "controller_switch_1"),
		ini_parser::instance().get<std::string>("key_config", "controller_switch_2")
	};
	if (is_key_pushed(key[0]) && is_key_rise(key[1])) {
		return new basic_controller();
	}
	return this;
}

void simple_controller::update_lock() {
	static bool is_lock_enable = false;
	std::string key {
		ini_parser::instance().get<std::string>("key_config", "lock")
	};
	if (is_key_rise(key)) {
		is_lock_enable = !is_lock_enable;
	}

	if (is_lock_enable) {
		_command["lock"] = 1.0f;
	} else {
		_command["lock"] = -1.0f;
	}
}

void simple_controller::update_movement() {
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

void simple_controller::update_angle_base() {
	_command["angle_base"] = _command["height"];
	_command["angle_left"]  = _command["angle"];
	_command["angle_right"] = _command["angle"];
}

std::string simple_controller::release() {
	_command["width"] = -1.0f;

	std::string grab_key {
		ini_parser::instance().get<std::string>("key_config", "grab")
	};

	double elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _time).count();
	if (elapsed_ms > ini_parser::instance().get<float>("setting", "release_wait_time_ms")) {
		if (is_key_rise(grab_key)) {
			return "height_low";
		}
	}

	return "release";
}

std::string simple_controller::height_low() {
	_state_name = "very_low";

	std::string grab_key {
		ini_parser::instance().get<std::string>("key_config", "grab")
	};
	if (is_key_rise(grab_key)){
		_time = std::chrono::system_clock::now();
		return "grab";
	}

	return "height_low";
}

std::string simple_controller::grab() {
	_command["width"] = 1.0f;

	double elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _time).count();
	if (elapsed_ms > ini_parser::instance().get<float>("setting", "release_wait_time_ms")) {
		return "height_adjust";
	}
	return "grab";
}

std::string simple_controller::height_adjust() {
	update_state_name();

	std::string grab_key {
		ini_parser::instance().get<std::string>("key_config", "grab")
	};
	if (is_key_rise(grab_key)){
		_time = std::chrono::system_clock::now();
		return "release";
	}

	return "height_adjust";
}

void simple_controller::update_state_name() {
	int index = controller_impl::read_arm_abilities_position_index();
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

void simple_controller::update_state_by_state_name() {
	std::string abilities[] {
		"length",
		"height",
		"angle"
	};

	for (const auto& i : abilities) {
		std::string key {
			"state" + std::to_string(_state_index_list[_state_name]) + "_" + i + "_index"
		};
		int index = ini_parser::instance().get<int>("arm_state", key);
		_command[i] = ini_parser::instance().get<float>("arm_" + i, "position" + std::to_string(index));
	}
}
