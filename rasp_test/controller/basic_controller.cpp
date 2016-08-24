/*
 * basiccontroller.cpp
 *
 *  Created on: 2016/07/18
 *      Author: u
 */

#include <controller/basic_controller.hpp>
#include <controller/simple_controller.hpp>
#include <ini_parser.hpp>
#include <iostream>
#include <functional>

const std::string basic_controller::_arm_abilities_name[] {
	"length",
	"width",
	"height",
	"angle"
};

const std::map<std::string, size_t> basic_controller::_arm_abilities_init_position_index {
	{"length", 0},
	{"width",  0},
	{"height", 0},
	{"angle",  0}
};

basic_controller::basic_controller()  {
	for (const auto& i : _arm_abilities_init_position_index) {
		_arm_abilities_position_index[i.first] = i.second;
	}
}

basic_controller::~basic_controller() {}

controller_impl* basic_controller::update() {
	update_ini_parser();
	update_movement();
	update_arm();
	return update_sequence();
}

void basic_controller::update_ini_parser() {
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

void basic_controller::update_movement() {
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

void basic_controller::update_arm() {
	ini_parser& ini {
		ini_parser::instance()
	};

	static bool prev_ib_state = false;
	bool ib_state = udpate_arm_index_and_adjustment();

	if (!ib_state && prev_ib_state) {
		// 操作が終わったら微調整分を適用してファイルに書き込む
		for (const auto& i : _arm_abilities_name) {
			std::string key {
				"position" + std::to_string(_arm_abilities_position_index[i])
			};

			if (_arm_adjustment[i] != 0.0f) {
				ini.set("arm_" + i, key, ini.get<float>("arm_" + i, key) + _arm_adjustment[i]);
			}
			_arm_adjustment[i] = 0.0f;
		}
		ini.write();
	}
	prev_ib_state = ib_state;

	// 各機能はインデックスと微調整に応じて値が設定される
	update_arm_abilities_position();
}

bool basic_controller::udpate_arm_index_and_adjustment() {
	ini_parser& ini {
		ini_parser::instance()
	};
	for (const auto& i : _arm_abilities_name) {
		std::string key {
			ini.get<std::string>("key_config", "IB_" + i)
		};
		if (is_key_pushed(key) && update_arm_abilities_position_index(i)) {
			update_arm_adjustment(i);
			return true;
		}
	}
	return false;
}

void basic_controller::update_arm_adjustment(std::string name) {
	ini_parser& ini {
		ini_parser::instance()
	};

	float adj = _normalized_controller_state[ini.get<std::string>("key_config", "arm_adjusting_+")];
	adj -= _normalized_controller_state[ini.get<std::string>("key_config", "arm_adjusting_-")];
	_arm_adjustment[name] += adj * ini.get<float>("command_coeff", "command_coeff_arm_adjustment");

	float pos = ini.get<float>("arm_" + name, "position" + std::to_string(_arm_abilities_position_index[name]));
	if (pos + _arm_adjustment[name] > 1.0f) {
		_arm_adjustment[name] = 1.0f - pos;
	} else if (pos + _arm_adjustment[name] < -1.0f) {
		_arm_adjustment[name] = -1.0f - pos;
	}
}

bool basic_controller::update_arm_abilities_position_index(std::string name) {
	int index = read_arm_abilities_position_index();
	if (index < 0) {
		return false;
	}
	std::string x_key {
		ini_parser::instance().get<std::string>("key_config", "height_high_x")
	};
	std::string y_key {
		ini_parser::instance().get<std::string>("key_config", "height_high_y")
	};
	float x = _normalized_controller_state[x_key];
	float y = _normalized_controller_state[y_key];
	float l = sqrt(x * x + y * y);
	if (l > _command_threshold) {
		index += ini_parser::instance().get<int>("key_config", "height_low_key_num");
	}

	int position_num = ini_parser::instance().get<int>("arm_" + name, "position_num");
	if (index >= position_num) {
		index = position_num - 1;
	}
	_arm_abilities_position_index[name] = index;
	return true;
}

void basic_controller::update_arm_abilities_position() {
	for (const auto& i : _arm_abilities_name) {
		std::string key {
			"position" + std::to_string(_arm_abilities_position_index[i])
		};
		_command[i] = ini_parser::instance().get<float>("arm_" + i, key);
		_command[i] += _arm_adjustment[i];
	}

	_command["angle_left"] = _command["angle"];
	_command["angle_right"] = _command["angle"];
}

controller_impl* basic_controller::update_sequence() {
	std::string key[] {
		ini_parser::instance().get<std::string>("key_config", "controller_switch_1"),
		ini_parser::instance().get<std::string>("key_config", "controller_switch_2")
	};
	if (is_key_pushed(key[0]) && is_key_rise(key[1])) {
		return new simple_controller();
	}

	return this;
}

