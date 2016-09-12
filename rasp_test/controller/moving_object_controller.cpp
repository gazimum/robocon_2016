/*
 * moving_object_controller.cpp
 *
 *  Created on: 2016/09/06
 *      Author: u
 */

#include <cmath>
#include <controller/moving_object_controller.hpp>
#include <ini_parser.hpp>
#include <utils.hpp>

moving_object_controller::moving_object_controller() {
	// TODO 自動生成されたコンストラクター・スタブ
}

moving_object_controller::~moving_object_controller() {
	// TODO Auto-generated destructor stub
}

void moving_object_controller::update_movement() {
	// 平行移動
	float vx = _normalized_controller_state[get_key_by_name("velocity_x")];
	float vy = _normalized_controller_state[get_key_by_name("velocity_y")];
	float av = _normalized_controller_state[get_key_by_name("turn_+")];
	av -= _normalized_controller_state[get_key_by_name("turn_-")];

	float coeff = ini_parser::instance().get<float>("command_coeff", "command_coeff_speed_mode1");
	if (is_key_pushed("speed_mode_2_switch_1") || is_key_pushed("speed_mode_2_switch_2")) {
		coeff = ini_parser::instance().get<float>("command_coeff", "command_coeff_speed_mode2");
	}

	float l = sqrt(vx * vx + vy * vy);
	float threshold = ini_parser::instance().get<float>("setting", "velocity_threshold");
	if (l > threshold) {
		vx *= coeff / l;
		vy *= coeff / l;
	} else {
		vx = vy = 0.0f;
	}
	utils::restrict_vector_to_unit_vector(vx, vy);
	_command["velocity_x"] = vx;
	_command["velocity_y"] = vy;

	threshold = ini_parser::instance().get<float>("setting", "angular_velocity_threshold");
	if (std::fabs(av) > threshold) {
		av *= coeff;
	} else {
		av = 0.0f;
	}
	_command["angular_velocity"]  = av;
}

