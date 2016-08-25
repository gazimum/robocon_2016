/*
 * moving_object.cpp
 *
 *  Created on: 2016/06/27
 *      Author: tomoya
 */

#include <ini_parser.hpp>
#include <map>
#include <string>
#include <robot/moving_object.hpp>
#include <robot/omni_wheel.hpp>
#include <server_shared_data.hpp>
#include <communication.hpp>
#include <controller/controller.hpp>
#include <pid/position_pid.hpp>

moving_object::moving_object() {}

moving_object::~moving_object() {}

void moving_object::update() {
	if (controller::instance().get("reload_ini_file") > 0.0f) {
		_omni_wheel.update_tire_frequency_pid_coeff();
	}

	// 平行移動の速度設定
	_omni_wheel.set_velocity(
		controller::instance().get("velocity_x"),
		controller::instance().get("velocity_y")
	);
	// 旋回速度設定
	_omni_wheel.set_angular_velocity(
		controller::instance().get("angular_velocity")
	);

	_omni_wheel.write();
}
