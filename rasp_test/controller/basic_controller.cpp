/*
 * basiccontroller.cpp
 *
 *  Created on: 2016/07/18
 *      Author: u
 */

#include <iostream>
#include <functional>
#include <controller/basic_controller.hpp>
#include <controller/simple_controller.hpp>
#include <ini_parser.hpp>
#include <potentiometer.hpp>

basic_controller::basic_controller()  {
}

basic_controller::~basic_controller() {}

controller_impl* basic_controller::update() {
	update_movement();
	update_arm();
	update_angle_base();
	return update_sequence();
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

void basic_controller::update_angle_base() {
	_command["angle_base"] = _command["height"];
	_command["angle_left"]  = _command["angle"];
	_command["angle_right"] = _command["angle"];
}

void basic_controller::update_arm() {
	ini_parser& ini {
		ini_parser::instance()
	};
	static bool prev_ib_state = false;
	bool ib_state = udpate_arm_index_and_adjustment();
	// IB:Identifier Button 識別ボタン

	if (!ib_state && prev_ib_state) {
		// 操作が終わったら微調整分を適用してファイルに書き込む
		write_ini_file();
	}
	prev_ib_state = ib_state;

	// 各機能はインデックスと微調整に応じて値が設定される
	update_arm_ability_position();
}

bool basic_controller::udpate_arm_index_and_adjustment() {
	ini_parser& ini {
		ini_parser::instance()
	};
	for (const auto& i : _arm_ability_name_dataset) {
		std::string ib_key {
			ini.get<std::string>("key_config", "IB_" + i)
		};
		if (is_key_pushed(ib_key) && update_arm_ability_position_index(i)) {
			// IB + index key
			update_arm_adjustment(i);
			teaching(i);

			// todo:delete
			std::cout << i << "" << _arm_adjustment[i] << std::endl;

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

	std::string key {
		"position" + std::to_string(_arm_ability_position_index_dataset[name])
	};
	float pos = ini.get<float>("arm_" + name, key);
	if (pos + _arm_adjustment[name] > 1.0f) {
		_arm_adjustment[name] = 1.0f - pos;
	} else if (pos + _arm_adjustment[name] < -1.0f) {
		_arm_adjustment[name] = -1.0f - pos;
	}
}

bool basic_controller::update_arm_ability_position_index(std::string name) {
	int index = read_arm_ability_position_index();
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
	_arm_ability_position_index_dataset[name] = index;
	return true;
}

void basic_controller::update_arm_ability_position() {
	for (const auto& i : _arm_ability_name_dataset) {
		std::string key {
			"position" + std::to_string(_arm_ability_position_index_dataset[i])
		};
		_command[i] = ini_parser::instance().get<float>("arm_" + i, key);
		_command[i] += _arm_adjustment[i];
	}
}

void basic_controller::teaching(std::string name) {
	ini_parser& ini {
		ini_parser::instance()
	};
	std::string key {
		ini.get<std::string>("key_config", "teaching")
	};
	if (is_key_rise(key)) {
		std::string key {
			"position" + std::to_string(_arm_ability_position_index_dataset[name])
		};
		float pos = potentiometer::instance().get_position(name);
		ini.set("arm_" + name, key, pos);
		_arm_adjustment[name] = 0.0f;
		write_ini_file();
	}
}

void basic_controller::write_ini_file() {
	ini_parser& ini {
		ini_parser::instance()
	};

	for (const auto& i : _arm_ability_name_dataset) {
		std::string key {
			"position" + std::to_string(_arm_ability_position_index_dataset[i])
		};

		if (_arm_adjustment[i] != 0.0f) {
			ini.set("arm_" + i, key, ini.get<float>("arm_" + i, key) + _arm_adjustment[i]);
		}
		_arm_adjustment[i] = 0.0f;
	}
	ini.write();
}

