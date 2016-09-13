/*
 * omniwheel.cpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#include <config.hpp>
#include <cmath>
#include <algorithm>
#include "omni_wheel.hpp"
#include <i2c/i2c.hpp>
#include <robot/wheel_odometry.hpp>
#include <dc_motor.hpp>
#include <controller/controller.hpp>
#include <pid/pid_manager.hpp>
#include <utils.hpp>

#include <serial_connected_mcu/serial_connected_mcu_master.hpp>

omni_wheel::omni_wheel() : _velocity_x(float()),
							   _velocity_y(float()),
							   _angular_velocity(float()),
							   _target_heading_rad(float()) {
	pid_manager<float>::instance().add<position_pid<float>>("omni_wheel_heading");
}

omni_wheel::~omni_wheel() {}

void omni_wheel::write() {
	size_t tire_num = config::instance().get<int>("omni_wheel", "tire_num");
	float p[tire_num];
	for (size_t i = 0; i < tire_num; ++i) {
		float theta = config::instance().get<float>("omni_wheel", "tire_" + std::to_string(i) + "_heading_deg");
		theta *= M_PI / 180.0f;
		// タイヤの向きベクトルと速度ベクトルの内積
		p[i] = _velocity_x * cos(theta) + _velocity_y * sin(theta);
	}
	// ロボットの角度制御
	_target_heading_rad += _angular_velocity;
	float e = _target_heading_rad - wheel_odometry::instance().get_heading_rad();
	utils::normalize_angle_rad(e); // [?, ?] -> [-pi, pi]]
	float mv = pid_manager<float>::instance().get("omni_wheel_heading").update(e);

	for (size_t i = 0; i < tire_num; ++i) {
		dc_motor::instance().set("wheel" + std::to_string(i), p[i] + mv);
	}
}

void omni_wheel::set_velocity(float vx, float vy) {
	utils::restrict_vector_to_unit_vector(vx, vy);
	_velocity_x = vx;
	_velocity_y = vy;
}

void omni_wheel::set_angular_velocity(float av) {
	_angular_velocity = av;
}

void omni_wheel::set_target_heading_rad(float heading_rad) {
	_target_heading_rad = heading_rad;
}
