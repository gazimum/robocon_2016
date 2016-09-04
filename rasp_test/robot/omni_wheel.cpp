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

#include <serial_connected_mcu/serial_connected_mcu_master.hpp>

omni_wheel::omni_wheel() : _velocity_x(float()),
							   _velocity_y(float()),
							   _angular_velocity(float()) {
	update_tire_frequency_pid_coeff();
	controller::instance().add_ini_file_value_reload_function(
			std::bind(&omni_wheel::update_tire_frequency_pid_coeff, this)
	);
}

omni_wheel::~omni_wheel() {}

void omni_wheel::write() {
	float p[_wheel_num];
	for (size_t i = 0; i < _wheel_num; ++i) {
		float theta = ini_parser::instance().get<float>("omni_wheel", "tire_" + std::to_string(i) + "_heading_deg");
		theta *= M_PI / 180.0f;
		// タイヤの向きベクトルと速度ベクトルの内積
		p[i] = _velocity_x * cos(theta) + _velocity_y * sin(theta);
		p[i] += _angular_velocity;
	}
	// 回転数制御
	for (size_t i = 0; i < _wheel_num; ++i) {
		//float f = _wheel_odometry->get_raw(i) * 1000.0f / ini_parser::instance().get<int>("encoder_profile", "encoder_resolution");
		float f = wheel_odometry::get_tire_advanced_speed_cm_per_sec(i);
		float target_f = p[i] * ini_parser::instance().get<float>("setting", "target_tire_frequency");

		float threshold = 0.01f;
		if (std::fabs(target_f) > threshold) {
			float e = target_f - f;
			p[i] = _tire_frequency_pid[i](e);
		} else {
			_tire_frequency_pid[i].init();
		}
		p[i] = std::max(-1.0f, std::min(p[i], 1.0f));
	}
	/*
	// 速度制御
	for (size_t i = 0; i < _wheel_num; ++i) {
		float v = wheel_odometry::get_tire_advanced_speed_cm_per_sec(i);
		float target_v = p[i] * ini_parser::instance().get<float>("setting", "target_tire_speed");

		float threshold = 0.01f;
		if (std::fabs(target_v) > threshold) {
			float e = target_v - v;
			p[i] = _tire_frequency_pid[i](e);
		} else {
			_tire_frequency_pid[i].init();
		}
		p[i] = std::max(-1.0f, std::min(p[i], 1.0f));
	}
	*/
	for (size_t i = 0; i < _wheel_num; ++i) {
		dc_motor::instance().set("wheel" + std::to_string(i), p[i]);
	}
}

void omni_wheel::set_velocity(float x, float y) {
	float l = x * x + y * y;
	if (l > 1.0f) { // ベクトルの大きさが1より大きいなら1に調整する
		l = sqrt(l);
		x /= l;
		y /= l;
	}
	_velocity_x = x;
	_velocity_y = y;
}

void omni_wheel::set_angular_velocity(float v) {
	_angular_velocity = std::max(-1.0f, std::min(1.0f, v));
}

void omni_wheel::update_tire_frequency_pid_coeff() {
	for (size_t i = 0; i < _wheel_num; ++i) {
		_tire_frequency_pid[i].update_coeff(
			ini_parser::instance().get<float>("pid_coeff", "omni_wheel_tire_frequency_pid_kp"),
			ini_parser::instance().get<float>("pid_coeff", "omni_wheel_tire_frequency_pid_ki"),
			ini_parser::instance().get<float>("pid_coeff", "omni_wheel_tire_frequency_pid_kd")
		);
	}
}
