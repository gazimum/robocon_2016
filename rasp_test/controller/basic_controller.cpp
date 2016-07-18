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


basic_controller::basic_controller() {
	// TODO 自動生成されたコンストラクター・スタブ

}

basic_controller::~basic_controller() {
	// TODO Auto-generated destructor stub
}

controller_impl* basic_controller::update(std::map<std::string, float>& command) {
	update_movement(command);
	update_arm(command);

	return this;
}

void basic_controller::update_movement(std::map<std::string, float>& command) {
	ini_parser& config{
		ini_parser::instance()
	};

	// 平行移動
	float vx = command[config.key_config<std::string>("velocity_x")];
	float vy = command[config.key_config<std::string>("velocity_y")];

	float l = vx * vx + vy * vy;
	if (l > 1.0f) {
		l = sqrt(l);
		vx /= l;
		vy /= l;
	}

	// 旋回
	_command["angular_velocity"]  = command[config.key_config<std::string>("turn_+")];
	_command["angular_velocity"] -= command[config.key_config<std::string>("turn_-")];
}

void basic_controller::update_arm(std::map<std::string, float>& command) {
	ini_parser& config{
		ini_parser::instance()
	};

	// * IB : Identifier Button (識別ボタン) : 腕の機能を識別するボタン
	// 1. IB						 : 状態を1ずつ遷移
	// 2. index + IB + (+ or - or 0) : 指定番号の状態に遷移.さらにそこから微調整可能.

	bool is_index_enable = update_arm_abilities_position_index(command);

	for (const auto& i : _arm_abilities_name) {
		_command[i] = command[config.key_config<std::string>("IB_" + i)];

		if (_command[i] > _command_threshold) {
			_arm_adjusting_values[i] += command[config.key_config<std::string>("arm_adjusting_+")];
			_arm_adjusting_values[i] -= command[config.key_config<std::string>("arm_adjusting_-")];

			if (is_index_enable) {
				_arm_abilities_position_index;
			} else {
				_arm_abilities_position_index;
			}
		}
	}
}

bool basic_controller::update_arm_abilities_position_index(std::map<std::string, float>& command) {
	ini_parser& config{
		ini_parser::instance()
	};

	for (size_t i = 0; i < config.setting<int>("arm_abilities_position_num"); ++i) {
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

