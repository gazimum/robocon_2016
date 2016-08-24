/*
 * wheel_odometry.cpp
 *
 *  Created on: 2016/07/06
 *      Author: tomoya
 */

#include <robot/wheel_odometry.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <cmath>
#include <ini_parser.hpp>

const float wheel_odometry::_tire_radius_cm = 5.31f;
const float wheel_odometry::_tire_encoder_gear_ratio = 9.0f / 32.0f;
const float wheel_odometry::_encoder_normalize_coeff = -1.0f;

wheel_odometry::wheel_odometry() : _encoder_raw_data_lpf{
											ini_parser::instance().get<float>("setting", "encoder_raw_data_lpf_p"),
											ini_parser::instance().get<float>("setting", "encoder_raw_data_lpf_p"),
											ini_parser::instance().get<float>("setting", "encoder_raw_data_lpf_p")
										} {}

wheel_odometry::~wheel_odometry() {}

float wheel_odometry::get_tire_frequency_kHz(int id) {
	return _tire_encoder_gear_ratio * get_raw(id) / ini_parser::instance().get<int>("setting", "encoder_resolution");
}

float wheel_odometry::get_tire_frequency_Hz(int id) {
	return get_tire_frequency_kHz(id) * 1000.0f;
}

float wheel_odometry::get_tire_advanced_speed_cm_per_sec(int id) {
	return get_tire_frequency_Hz(id) * 2.0f * M_PI * _tire_radius_cm;
}

float wheel_odometry::get_raw(int index) {
	int id = serial_connected_mcu::ENCODER_SPEED1 + index;
	return _encoder_normalize_coeff * serial_connected_mcu::serial_connected_mcu_master::instance().get_raw(id);
}

float wheel_odometry::get_heading_rad() {
	return 0.0f;
}
