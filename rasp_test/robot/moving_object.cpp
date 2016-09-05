/*
 * moving_object.cpp
 *
 *  Created on: 2016/06/27
 *      Author: tomoya
 */

#include <ini_parser.hpp>
#include <map>
#include <cmath>
#include <string>
#include <robot/moving_object.hpp>
#include <robot/omni_wheel.hpp>
#include <server_shared_data.hpp>
#include <communication.hpp>
#include <controller/controller.hpp>
#include <pid/position_pid.hpp>
#include <robot/wheel_odometry.hpp>

moving_object::moving_object() {}

moving_object::~moving_object() {}

void moving_object::update() {
	float vx = controller::instance().get("velocity_x");
	float vy = controller::instance().get("velocity_y");
	float av = controller::instance().get("angular_velocity");

	_omni_wheel.set_velocity(vx, vy);
	_omni_wheel.set_angular_velocity(av);
	_omni_wheel.write();

	// 移動する状態だったらLPFは有効化
	float l = sqrt(vx * vx + vy * vy);
	if (l > ini_parser::instance().get<float>("lpf", "lpf_enable_threshold_velocity")) {
		wheel_odometry::instance().enable_lpf();
		return;
	}
	if (std::fabs(av) > ini_parser::instance().get<float>("lpf", "lpf_enable_threshold_angular_velocity")) {
		wheel_odometry::instance().enable_lpf();
		return;
	}
	// 移動する状態でないならLPFは無効化
	wheel_odometry::instance().disable_lpf();
}
