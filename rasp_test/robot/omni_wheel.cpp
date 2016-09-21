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
#include <pid/position_pid.hpp>
#include <utils.hpp>

#include <serial_connected_mcu/serial_connected_mcu_master.hpp>

omni_wheel::omni_wheel() {
	pid_manager<float>::instance().add<position_pid<float>>("omni_wheel_heading");
}

omni_wheel::~omni_wheel() {}

void omni_wheel::write() {
	size_t tire_num = config::instance().get<int>("omni_wheel", "tire_num");
	float parallel[tire_num];
	for (size_t i = 0; i < tire_num; ++i) {
		float theta = config::instance().get<float>("omni_wheel", "tire_" + std::to_string(i) + "_heading_deg");
		theta *= M_PI / 180.0f;
		// タイヤの向きベクトルと速度ベクトルの内積
		parallel[i] = _velocity_x * cos(theta) + _velocity_y * sin(theta);
	}
	// ロボットの角度制御
	float e = _target_heading_rad - wheel_odometry::instance().get_heading_rad();
	utils::normalize_angle_rad(e); // [?, ?] -> [-pi, pi]]
	float mv = pid_manager<float>::instance().get("omni_wheel_heading").update(e);

	for (size_t i = 0; i < tire_num; ++i) {
		// 「タイヤの力のベクトル」と「正回転方向に対するタイヤの向き単位ベクトル」の内積
		float tire_position_deg = config::instance().get<float>("omni_wheel", "tire_" + std::to_string(i) + "_position_deg");
		float tire_heading_deg  = config::instance().get<float>("omni_wheel", "tire_" + std::to_string(i) + "_heading_deg");
		float between_deg = tire_position_deg + 90.0f - tire_heading_deg;
		float turn = mv * cos(between_deg * M_PI / 180.0f);
		dc_motor::instance().set("wheel_" + std::to_string(i), parallel[i] + turn);
	}
}
