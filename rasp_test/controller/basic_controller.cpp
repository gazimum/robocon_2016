/*
 * basiccontroller.cpp
 *
 *  Created on: 2016/07/18
 *      Author: u
 */

#include <controller/basic_controller.hpp>
#include <ini_parser.hpp>

const std::string basic_controller::_arm_abilities_name[] {
	"lengther",
	"widener",
	"angle_adjuster"
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
	update_movement(normalized_controller_state);
	update_arm(normalized_controller_state);
	return update_sequence(normalized_controller_state);
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

	// 旋回
	_command["angular_velocity"]  = normalized_controller_state[config.key_config<std::string>("turn_+")];
	_command["angular_velocity"] -= normalized_controller_state[config.key_config<std::string>("turn_-")];
}

void basic_controller::update_arm(std::map<std::string, float>& normalized_controller_state) {
	ini_parser& config{
		ini_parser::instance()
	};

	// まず各機能はインデックスに応じて値が設定される
	update_arm_abilities_position();

	// * IB : Identifier Button (識別ボタン) : 腕の機能が割り当てられたボタン
	// 1. IB						 : 状態を1ずつ遷移
	// 2. index + IB + (+ or - or 0) : 指定番号の状態に遷移.さらにそこから微調整可能.
	for (const auto& i : _arm_abilities_name) {
		float ib = normalized_controller_state[config.key_config<std::string>("IB_" + i)];

		if (ib > _command_threshold) {
			if (update_arm_abilities_position_index(normalized_controller_state)) {
				// 2. index + IB + (+ or - or 0) : 指定番号の状態に遷移.さらにそこから微調整可能.
				_arm_adjusting_values[i] += normalized_controller_state[config.key_config<std::string>("arm_adjusting_+")];
				_arm_adjusting_values[i] -= normalized_controller_state[config.key_config<std::string>("arm_adjusting_-")];

				int index =  _command["arm_abilities_position_index"];
				std::string key {
					i + "_position_" + std::to_string(index)
				};
				_command[i] = config.setting<float>(key);
				_command[i] += _arm_adjusting_values[i];
			} else {
				// 1. IB : 状態を1ずつ遷移
				_arm_abilities_position_index;
			}
		}
	}
}

controller_impl* basic_controller::update_sequence(std::map<std::string, float>& command) {
	return this;
}

bool basic_controller::update_arm_abilities_position_index(std::map<std::string, float>& command) {
	ini_parser& config{
		ini_parser::instance()
	};

	for (int i = 0; i < config.setting<int>("arm_abilities_position_num"); ++i) {
		std::string key {
			"arm_abilities_position_index_" + std::to_string(i)
		};
		if (command[config.key_config<std::string>(key)] > _command_threshold) {
			_command["arm_abilities_position_index"] = i;
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
		_command[i] = ini_parser::instance().setting<float>(key);
		_command[i] += _arm_adjusting_values[i];
	}
}

