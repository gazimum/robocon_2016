/*
 * omniwheel.cpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#include <cmath>
#include <algorithm>
#include "omni_wheel.hpp"
#include <i2c/i2c.hpp>
#include <robot/wheel_odometry.hpp>
#include <ini_parser.hpp>
#include <dc_motor.hpp>
#include <controller/controller.hpp>
#include <pid/pid_manager.hpp>

#include <serial_connected_mcu/serial_connected_mcu_master.hpp>

omni_wheel::omni_wheel() : _velocity_x(float()),
							   _velocity_y(float()),
							   _angular_velocity(float()),
							   _target_heading_rad(float()) {
	pid_manager::instance().add_pid<position_pid<float>>("omni_wheel_heading");
}

omni_wheel::~omni_wheel() {}

namespace {

static void normalize(float& theta) {
	while (theta > M_PI) {
		theta -= 2.0f * M_PI;
	}
	while (theta < -M_PI) {
		theta += 2.0f * M_PI;
	}
}

}

void omni_wheel::write() {
	size_t tire_num = ini_parser::instance().get<int>("omni_wheel", "tire_num");
	float p[tire_num];
	for (size_t i = 0; i < tire_num; ++i) {
		float theta = ini_parser::instance().get<float>("omni_wheel", "tire_" + std::to_string(i) + "_heading_deg");
		theta *= M_PI / 180.0f;
		// タイヤの向きベクトルと速度ベクトルの内積
		p[i] = _velocity_x * cos(theta) + _velocity_y * sin(theta);
	}
	// ロボットの角度制御
	_target_heading_rad += _angular_velocity;
	float e = _target_heading_rad - wheel_odometry::instance().get_heading_rad();
	normalize(e);
	float mv = pid_manager::instance().get_pid("omni_wheel_heading")->update(e);

	for (size_t i = 0; i < tire_num; ++i) {
		dc_motor::instance().set("wheel" + std::to_string(i), p[i] + mv);
	}
}

void omni_wheel::set_velocity(float x, float y) {
	float l = sqrt(x * x + y * y);
	if (l > 1.0f) { // ベクトルの大きさが1より大きいなら1に調整する
		x /= l;
		y /= l;
	}
	_velocity_x = x;
	_velocity_y = y;
}

void omni_wheel::set_angular_velocity(float v) {
	_angular_velocity = std::max(-1.0f, std::min(v, 1.0f));
}

void omni_wheel::set_target_heading_rad(float heading_rad) {
	_target_heading_rad = heading_rad;
}
