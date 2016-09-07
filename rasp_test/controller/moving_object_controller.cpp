/*
 * moving_object_controller.cpp
 *
 *  Created on: 2016/09/06
 *      Author: u
 */

#include <cmath>
#include <controller/moving_object_controller.hpp>

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

	float l = vx * vx + vy * vy;
	if (l > 1.0f) {
		l = sqrt(l);
		vx /= l;
		vy /= l;
	}
	_command["velocity_x"] = vx;
	_command["velocity_y"] = vy;

	// 旋回
	_command["angular_velocity"]  = _normalized_controller_state[get_key_by_name("turn_+")];
	_command["angular_velocity"] -= _normalized_controller_state[get_key_by_name("turn_-")];
}

