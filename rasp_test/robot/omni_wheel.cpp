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

static void normalize(float& theta) {
	while (theta > M_PI) {
		theta -= 2.0f * M_PI;
	}
	while (theta < -M_PI) {
		theta += 2.0f * M_PI;
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
	//std::cout << e << " " << wheel_odometry::instance().get_heading_rad() << std::endl;
	float mv = 0.0f;
	float threshold = 0.005f;
	//if (std::abs(e) > threshold) {
		mv = _heading_pid.update(e);
	/*} else {
		_heading_pid.init();
	}*/
	mv = std::max(-1.0f, std::min(mv, 1.0f));

	// 角度制御 平行移動 等価
	/*
	float max_p = 1.0f;
	for (size_t i = 0; i < tire_num; ++i) {
		p[i] += mv;
		if (std::fabs(p[i]) > max_p) {
			max_p = std::fabs(p[i]);
		}
	}
	*/
	// 角度制御優先
	/*
	float max_p = 0.0f;
	bool is_over = false;
	for (size_t i = 0; i < tire_num; ++i) {
		if (std::fabs(p[i] + mv) > 1.0f) {
			is_over = true;
			if (std::fabs(p[i]) > max_p) {
				max_p = p[i];
			}
		}
	}
	*/
	/* 平行移動優先
	for (size_t i = 0; i < tire_num; ++i) {
		if (p[i] + mv > 1.0f) {
			mv = 1.0f - p[i];
		} else if (p[i] + mv < -1.0f) {
			mv = -1.0f - p[i];
		}
	}
	*/
	/*
	float coeff = 1.0f;
	if (is_over) {
		if (max_p > 0.0f) {
			// coeff * max_p + mv = 1.0
			coeff = (mv - 1.0f) / max_p;
		} else {
			// coeff * max_p + mv = -1.0
			coeff = (mv + 1.0f) / max_p;
		}
		coeff *= max_p;
	}
	*/
	for (size_t i = 0; i < tire_num; ++i) {
		//dc_motor::instance().set("wheel" + std::to_string(i), p[i] * coeff + mv);
		//dc_motor::instance().set("wheel" + std::to_string(i), p[i] / max_p);
		//dc_motor::instance().set("wheel" + std::to_string(i), p[i] + mv * (1.0f - max_p) * (1.0f - max_p));
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

void omni_wheel::update_tire_frequency_pid_coeff() {
	/*
	size_t tire_num = ini_parser::instance().get<int>("omni_wheel", "tire_num");
	for (size_t i = 0; i < tire_num; ++i) {
		_tire_frequency_pid[i].update_coeff(
			ini_parser::instance().get<float>("pid_coeff", "omni_wheel_tire_frequency_pid_kp"),
			ini_parser::instance().get<float>("pid_coeff", "omni_wheel_tire_frequency_pid_ki"),
			ini_parser::instance().get<float>("pid_coeff", "omni_wheel_tire_frequency_pid_kd")
		);
	}
	*/
	_heading_pid.update_coeff(
		ini_parser::instance().get<float>("pid_coeff", "omni_wheel_heading_pid_kp"),
		ini_parser::instance().get<float>("pid_coeff", "omni_wheel_heading_pid_ki"),
		ini_parser::instance().get<float>("pid_coeff", "omni_wheel_heading_pid_kd")
	);
}
