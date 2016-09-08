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
#include <iomanip>

#include <serial_connected_mcu/serial_connected_mcu_master.hpp>

omni_wheel::omni_wheel() : _velocity_x(float()),
							   _velocity_y(float()),
							   _angular_velocity(float()),
							   _target_heading_rad(float()) {
	update_tire_frequency_pid_coeff();
	controller::instance().add_reload_ini_file_value_function(
			std::bind(&omni_wheel::update_tire_frequency_pid_coeff, this)
	);
}

omni_wheel::~omni_wheel() {}

void normalize(float& theta) {
	while (theta > M_PI) {
		theta -= 2.0f * M_PI;
	}
	while (theta < -M_PI) {
		theta += 2.0f * M_PI;
	}
}

void omni_wheel::write() {
	float p[_wheel_num];
	for (size_t i = 0; i < _wheel_num; ++i) {
		float theta = ini_parser::instance().get<float>("omni_wheel", "tire_" + std::to_string(i) + "_heading_deg");
		theta *= M_PI / 180.0f;
		// タイヤの向きベクトルと速度ベクトルの内積
		p[i] = _velocity_x * cos(theta) + _velocity_y * sin(theta);
	}
	// ロボットの角度制御
	_target_heading_rad += _angular_velocity;
	float e = _target_heading_rad - wheel_odometry::instance().get_heading_rad();
	normalize(e);
	std::cout << e << std::endl;
	float mv = 0.0f;
	float threshold = 0.01f;
	if (std::abs(e) > threshold) {
		mv = _heading_pid.update(e);
	} else {
		_heading_pid.init();
	}
	for (size_t i = 0; i < _wheel_num; ++i) {
		if (p[i] + mv > 1.0f) {
			mv = 1.0f - p[i];
		} else if (p[i] + mv < -1.0f) {
			mv = -1.0f - p[i];
		}
	}
	for (size_t i = 0; i < _wheel_num; ++i) {
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
	_angular_velocity = std::max(-1.0f, std::min(1.0f, v));
}

void omni_wheel::set_target_heading_rad(float heading_rad) {
	_target_heading_rad = heading_rad;
}

void omni_wheel::update_tire_frequency_pid_coeff() {
	for (size_t i = 0; i < _wheel_num; ++i) {
		_tire_frequency_pid[i].update_coeff(
			ini_parser::instance().get<float>("pid_coeff", "omni_wheel_tire_frequency_pid_kp"),
			ini_parser::instance().get<float>("pid_coeff", "omni_wheel_tire_frequency_pid_ki"),
			ini_parser::instance().get<float>("pid_coeff", "omni_wheel_tire_frequency_pid_kd")
		);
	}
	_heading_pid.update_coeff(
			ini_parser::instance().get<float>("pid_coeff", "omni_wheel_heading_pid_kp"),
			ini_parser::instance().get<float>("pid_coeff", "omni_wheel_heading_pid_ki"),
			ini_parser::instance().get<float>("pid_coeff", "omni_wheel_heading_pid_kd")
	);
}
