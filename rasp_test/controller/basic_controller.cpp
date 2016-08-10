/*
 * basiccontroller.cpp
 *
 *  Created on: 2016/07/18
 *      Author: u
 */

#include <controller/basic_controller.hpp>
#include <ini_parser.hpp>
#include <iostream>

const std::string basic_controller::_arm_abilities_name[] {
	"length",
	"width",
	"height",
	"angle"
};

const std::map<std::string, size_t> basic_controller::_arm_abilities_init_position_index {
	{_arm_abilities_name[0], 0},
	{_arm_abilities_name[1], 0},
	{_arm_abilities_name[2], 0}
};

basic_controller::basic_controller() {
	for (auto&& i : _arm_abilities_position_index) {
		i.second = _arm_abilities_init_position_index.at(i.first);
	}
}

basic_controller::~basic_controller() {
	// TODO Auto-generated destructor stub
}

controller_impl* basic_controller::update(std::map<std::string, float>& normalized_controller_state) {
	update_ini_parser(normalized_controller_state);
	update_movement(normalized_controller_state);
	update_arm(normalized_controller_state);
	return update_sequence(normalized_controller_state);
}

void basic_controller::update_ini_parser(std::map<std::string, float>& normalized_controller_state) {
	static float prev_state = false;

	std::string key {
		ini_parser::instance().key_config<std::string>("reload_ini_file")
	};
	float state = (normalized_controller_state[key] > _command_threshold);

	if (state && !prev_state) {
		ini_parser::instance().init();
	}
	prev_state = state;
}

void basic_controller::update_movement(std::map<std::string, float>& normalized_controller_state) {
	ini_parser& config{
		ini_parser::instance()
	};

	// 平行移動
	float vx = normalized_controller_state[config.key_config<std::string>("velocity_x")];
	float vy = normalized_controller_state[config.key_config<std::string>("velocity_y")];

	float l = vx * vx + vy * vy;
	if (l > 1.0f) {
		l = sqrt(l);
		vx /= l;
		vy /= l;
	}
	_command["velocity_x"] = vx;
	_command["velocity_y"] = vy;

	// 旋回
	_command["angular_velocity"]  = normalized_controller_state[config.key_config<std::string>("turn_+")];
	_command["angular_velocity"] -= normalized_controller_state[config.key_config<std::string>("turn_-")];
}

void basic_controller::update_arm(std::map<std::string, float>& normalized_controller_state) {
	ini_parser& config{
		ini_parser::instance()
	};

	static bool prev_ib_state = false;
	bool ib_state = udpate_arm_index_and_adjustment(normalized_controller_state);

	if (!ib_state && prev_ib_state) {
		// 操作が終わったら微調整分を適用してファイルに書き込む
		for (const auto& i : _arm_abilities_name) {
			std::string key {
				i + "_position_" + std::to_string(_arm_abilities_position_index[i])
			};

			if (_arm_adjustment[i] != 0.0f) {
				config.set_setting(key, config.setting<float>(key) + _arm_adjustment[i]);
			}
			_arm_adjustment[i] = 0.0f;
		}
	}
	prev_ib_state = ib_state;

	// 各機能はインデックスと微調整に応じて値が設定される
	update_arm_abilities_position();
}

bool basic_controller::udpate_arm_index_and_adjustment(std::map<std::string, float>& normalized_controller_state) {
	static std::string prev_enabled_ib_name;

	ini_parser& config{
		ini_parser::instance()
	};

	// * IB : Identifier Button (識別ボタン) : 腕の機能が割り当てられたボタン
	// 1. IB : 状態を1ずつ遷移
	// 2. index + IB + (+ or - or nothing) : 指定番号の状態に遷移.さらにそこから微調整可能.
	std::map<std::string, bool> ib_buttons_state;
	for (const auto& i : _arm_abilities_name) {
		std::string key{
			config.key_config<std::string>("IB_" + i)
		};
		ib_buttons_state[key] = (normalized_controller_state[key] > _command_threshold);
	}

	for (const auto& i : _arm_abilities_name) {
		std::string key{
			config.key_config<std::string>("IB_" + i)
		};
		if (ib_buttons_state[key]) {
			if (update_arm_abilities_position_index(normalized_controller_state, i)) {
				// 2. index + IB + (+ or - or 0) : 指定番号の状態に遷移.さらにそこから微調整可能.
				update_arm_adjustment(normalized_controller_state, i);
			} else if (!_prev_ib_buttons_state[key]) {
				// 	立ち上がりなら
				// 1. IB : 状態を1ずつ遷移
				if (++_arm_abilities_position_index[i] >= config.setting<size_t>("arm_" + i + "_position_num")) {
					_arm_abilities_position_index[i] = 0;
				}
			}

			_prev_ib_buttons_state = ib_buttons_state;

			bool is_ib_state_changed = (i == prev_enabled_ib_name);
			is_ib_state_changed &= (i.size() != 0);

			prev_enabled_ib_name = i;
			return is_ib_state_changed;
		}
	}
	_prev_ib_buttons_state = ib_buttons_state;

	prev_enabled_ib_name.erase();
	return false;
}

void basic_controller::update_arm_adjustment(std::map<std::string, float> normalized_controller_state, std::string name) {
	ini_parser& config{
		ini_parser::instance()
	};

	float adj = normalized_controller_state[config.key_config<std::string>("arm_adjusting_+")];
	adj -= normalized_controller_state[config.key_config<std::string>("arm_adjusting_-")];
	_arm_adjustment[name] += adj * config.setting<float>("command_coeff_arm_adjustment");

	float p = config.setting<float>(name + "_position_" + std::to_string(_arm_abilities_position_index[name]));
	if (p + _arm_adjustment[name] > 1.0f) {
		_arm_adjustment[name] = 1.0f - p;
	} else if (p + _arm_adjustment[name] < -1.0f) {
		_arm_adjustment[name] = -1.0f - p;
	}
}

controller_impl* basic_controller::update_sequence(std::map<std::string, float>& normalized_controller_state) {
	return this;
}

bool basic_controller::update_arm_abilities_position_index(std::map<std::string, float>& normalized_controller_state, std::string name) {
	ini_parser& config{
		ini_parser::instance()
	};

	for (int i = 0; i < config.setting<int>("arm_" + name + "_position_num"); ++i) {
		std::string key {
			"arm_abilities_position_index_" + std::to_string(i)
		};
		if (normalized_controller_state[config.key_config<std::string>(key)] > _command_threshold) {
			_arm_abilities_position_index[name] = i;
			return true;
		}
	}

	return false;
}

void basic_controller::update_arm_abilities_position() {
	for (const auto& i : _arm_abilities_name) {
		std::string key {
			i + "_position_" + std::to_string(_arm_abilities_position_index[i])
		};

		// todo : remove this when beta mode end
		if (i == "angle") {
			_command[i] = ini_parser::instance().setting<float>(key);
			_command[i] += _arm_adjustment[i];
		} else if ((i == "length") || (i == "width") || (i == "height")) {
			_command[i] = _arm_adjustment[i];
		}
	}
}

