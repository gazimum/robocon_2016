/*
 * basiccontroller.cpp
 *
 *  Created on: 2016/07/18
 *      Author: u
 */

#include <config.hpp>
#include <iostream>
#include <functional>
#include <controller/basic_controller.hpp>
#include <controller/simple_controller.hpp>
#include <controller/flexible_controller.hpp>
#include <potentiometer.hpp>
#include <i2c/i2c.hpp>

basic_controller::basic_controller()  {}

basic_controller::~basic_controller() {}

controller_impl* basic_controller::update() {
	i2c::instance().set("debug_0", 0x02);

	update_movement();
	update_arm();
	return update_sequence();
}

controller_impl* basic_controller::update_sequence() {
	if (is_key_pushed("controller_switch_3") && is_key_rise("controller_switch_1")) {
		return new flexible_controller;
	}
	return this;
}

void basic_controller::update_arm() {
	// IB:Identifier Button 識別ボタン
	static bool is_ib_pushed_prev = false;
	bool is_ib_pushed = udpate_arm_index_and_adjustment();

	if (!is_ib_pushed && is_ib_pushed_prev) {
		// 操作が終わったら微調整分を適用してファイルに書き込む
		write_ini_file();
	}
	is_ib_pushed_prev = is_ib_pushed;

	// 各機能の値はインデックスと微調整に応じて設定される
	// B1257
	update_arm_ability_position();
}

bool basic_controller::udpate_arm_index_and_adjustment() {
	for (const auto& i : _arm_ability_name_dataset) {
		if (is_key_pushed("IB_" + i)) {
			update_arm_ability_position_index(i);
			update_arm_adjustment(i);
			teaching(i);

			// todo:delete
			std::cout << i << "" << _arm_adjustment[i] << std::endl;

			return true;
		}
	}
	return false;
}

bool basic_controller::update_arm_ability_position_index(std::string name) {
	int index = read_arm_ability_position_index();
	if (index < 0) {
		return false;
	}
	float x = _normalized_controller_state[get_key_by_name("arm_index_mode_2_switch_2")];
	float y = _normalized_controller_state[get_key_by_name("arm_index_mode_2_switch_3")];
	float l = sqrt(x * x + y * y);
	if (l > _command_threshold) {
		index += config::instance().get<int>("key_config", "arm_index_mode_1_key_num");
	}

	int position_num = config::instance().get<int>(name, "position_num");
	if (index >= position_num) {
		index = position_num - 1;
	}
	_arm_ability_position_index_dataset[name] = index;
	return true;
}

void basic_controller::update_arm_adjustment(std::string name) {
	float adj = _normalized_controller_state[get_key_by_name("arm_adjusting_+")];
	adj -= _normalized_controller_state[get_key_by_name("arm_adjusting_-")];
	adj *= config::instance().get<float>("command_coeff", "command_coeff_arm_adjustment");
	_arm_adjustment[name] += adj;

	std::string value_key {
		"position_" + std::to_string(_arm_ability_position_index_dataset[name])
	};
	float pos = config::instance().get<float>(name, value_key);
	if (pos + _arm_adjustment[name] > 1.0f) {
		_arm_adjustment[name] = 1.0f - pos;
	} else if (pos + _arm_adjustment[name] < -1.0f) {
		_arm_adjustment[name] = -1.0f - pos;
	}
}

void basic_controller::update_arm_ability_position() {
	for (const auto& i : _arm_ability_name_dataset) {
		std::string value_key {
			"position_" + std::to_string(_arm_ability_position_index_dataset[i])
		};
		_command[i] = config::instance().get<float>(i, value_key);
		_command[i] += _arm_adjustment[i];
	}
}

void basic_controller::teaching(std::string name) {
	if (name == "arm_angle") {
		return;
	}

	if (is_key_rise("teaching")) {
		std::string value_key {
			"position_" + std::to_string(_arm_ability_position_index_dataset[name])
		};
		float pos = potentiometer::instance().get_position(name);
		config::instance().set(name, value_key, pos);
		_arm_adjustment[name] = 0.0f;
		write_ini_file();
	}
}

void basic_controller::write_ini_file() {
	for (const auto& i : _arm_ability_name_dataset) {
		if (_arm_adjustment[i] != 0.0f) {
			std::string value_key {
				"position_" + std::to_string(_arm_ability_position_index_dataset[i])
			};
			float pos = config::instance().get<float>(i, value_key);
			config::instance().set(i, value_key, pos + _arm_adjustment[i]);
		}
		_arm_adjustment[i] = 0.0f;
	}
	config::instance().write();
}

