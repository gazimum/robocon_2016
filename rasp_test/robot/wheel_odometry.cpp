/*
 * wheel_odometry.cpp
 *
 *  Created on: 2016/07/06
 *      Author: tomoya
 */

#include <config.hpp>
#include <robot/wheel_odometry.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <cmath>
#include <string>
#include <cstdint>
#include <controller/controller.hpp>
#include <iostream>
#include <utils.hpp>
#include <lpf/lpf_manager.hpp>

wheel_odometry::wheel_odometry() : _prev_raw(new float[config::instance().get<int>("encoder_profile", "encoder_num")]),
										_raw_offset(new float[config::instance().get<int>("encoder_profile", "encoder_num")]) {}

float wheel_odometry::get_tire_frequency_kHz(int index) {
	return get_raw(index) / config::instance().get<int>("encoder_profile", "encoder_resolution");
}

float wheel_odometry::get_tire_frequency_Hz(int index) {
	return get_tire_frequency_kHz(index) * 1000.0f;
}

float wheel_odometry::get_tire_advanced_speed_cm_per_sec(int index) {
	float circumference_cm = 2.0f * M_PI;
	circumference_cm *= config::instance().get<float>("omni_wheel", "tire_radius_cm");
	return get_tire_frequency_Hz(index) * circumference_cm;
}

float wheel_odometry::get_raw(int index) {
	float coeff = config::instance().get<float>("encoder_profile", "encoder_coeff_" + std::to_string(index));
	int id = serial_connected_mcu::ENCODER1 + index;
	float raw = coeff * serial_connected_mcu::serial_connected_mcu_master::instance().get_raw(id);
	if (raw - _prev_raw[index] >= INT16_MAX) { 			// マイナスにオーバーフロー
		_raw_offset[index] += INT16_MIN - INT16_MAX;
	} else if (raw - _prev_raw[index] <= INT16_MIN) {	// プラスにオーバーフロー
		_raw_offset[index] += INT16_MAX - INT16_MIN;
	}
	_prev_raw[index] = raw;
	return raw + _raw_offset[index];
}

float wheel_odometry::get_heading_rad() {
	float l = 0.0f;
	for (size_t i = 0; i < config::instance().get<int>("encoder_profile", "encoder_num"); ++i) {
		l += get_raw(i);
	}
	l /= config::instance().get<float>("encoder_profile", "encoder_resolution");
	l *= 2.0f * M_PI * config::instance().get<float>("omni_wheel", "tire_radius_cm");
	l /= config::instance().get<float>("encoder_profile", "encoder_num");
	float heading_rad = l / config::instance().get<float>("omni_wheel", "body_radius_cm");
	utils::normalize_angle_rad(heading_rad);
	return heading_rad;
}
