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

moving_object::moving_object() : _lpf_velocity_x(
										ini_parser::instance().get<float>("setting", "velocity_x_lpf_p")
									 ),
									 _lpf_velocity_y(
										ini_parser::instance().get<float>("setting", "velocity_y_lpf_p")
									 ),
									 _lpf_angular_velocity(
										ini_parser::instance().get<float>("setting", "angular_velocity_lpf_p")
									 ) {}

moving_object::~moving_object() {}

void moving_object::update() {
	// 平行移動の速度設定
	float target_vx = controller::instance().get("velocity_x");
	float target_vy = controller::instance().get("velocity_y");
	//float target_vx = _lpf_velocity_x(controller::instance().get("velocity_x"));
	//float target_vy = _lpf_velocity_y(controller::instance().get("velocity_y"));

	if (controller::instance().get("reload_ini_file") > 0.0f) {
		_omni_wheel.set_tire_frequency_pid_coeff(
			ini_parser::instance().get<float>("setting", "omni_wheel_tire_frequency_pid_kp"),
			ini_parser::instance().get<float>("setting", "omni_wheel_tire_frequency_pid_ki"),
			ini_parser::instance().get<float>("setting", "omni_wheel_tire_frequency_pid_kd")
		);
	}

	_omni_wheel.set_velocity(
		target_vx,//_lpf_velocity_x(target_vx),//_pid_velocity_x(target_vx - 0.0f),
		target_vy//_lpf_velocity_y(target_vy)//_pid_velocity_y(target_vy - 0.0f)
	);
	// 旋回速度設定
	_omni_wheel.set_angular_velocity(
		controller::instance().get("angular_velocity")//_lpf_angular_velocity(controller::instance().get("angular_velocity"))
	);

	_omni_wheel.write();
}
