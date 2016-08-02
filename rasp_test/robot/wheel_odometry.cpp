/*
 * wheel_odometry.cpp
 *
 *  Created on: 2016/07/06
 *      Author: tomoya
 */

#include <robot/wheel_odometry.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <cmath>

#include <iostream>

const int wheel_odometry::_encoder_resolution = 300 * 4;
const float wheel_odometry::_tire_radius_cm = 10.0f;

wheel_odometry::wheel_odometry() {
	// TODO 自動生成されたコンストラクター・スタブ

}

wheel_odometry::~wheel_odometry() {
	// TODO Auto-generated destructor stub
}

float wheel_odometry::get_tire_frequency_kHz(int id) {
	return serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::ENCODER_SPEED1 + id) / _encoder_resolution;
}

float wheel_odometry::get_tire_advanced_speed_cm_per_sec(int id) {
	return get_tire_frequency_kHz(id) * 2 * M_PI * _tire_radius_cm;
}

float wheel_odometry::get_heading_rad() {
	return 0.0f;
}
