/*
 * wheel_odometry.cpp
 *
 *  Created on: 2016/07/06
 *      Author: tomoya
 */

#include <robot/wheel_odometry.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <cmath>
#include <string>
#include <ini_parser.hpp>

float wheel_odometry::get_tire_frequency_kHz(int id) {
	return get_raw(id) / ini_parser::instance().get<int>("setting", "encoder_resolution");
}

float wheel_odometry::get_tire_frequency_Hz(int id) {
	return get_tire_frequency_kHz(id) * 1000.0f;
}

float wheel_odometry::get_tire_advanced_speed_cm_per_sec(int id) {
	float circumference_cm = 2.0f * M_PI;
	circumference_cm *= ini_parser::instance().get<float>("omni_wheel", "tire_radius_cm");
	return get_tire_frequency_Hz(id) * circumference_cm;
}

float wheel_odometry::get_raw(int index) {
	int id = serial_connected_mcu::ENCODER_SPEED1 + index;
	float coeff = ini_parser::instance().get<float>("encoder_profile", "encoder_coeff" + std::to_string(index));
	return coeff * serial_connected_mcu::serial_connected_mcu_master::instance().get_raw(id);
}

float wheel_odometry::get_heading_rad() {
	return 0.0f;
}
