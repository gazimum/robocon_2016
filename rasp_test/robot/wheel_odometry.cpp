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
#include <controller/controller.hpp>

wheel_odometry::wheel_odometry() : _is_lpf_enable(true) {
	for (size_t i = 0; i < ini_parser::instance().get<int>("encoder_profile", "encoder_num"); ++i) {
		_lpf_dataset.insert(
				std::pair<int, lpf<float>>(i, {})
		);
	}
	init();
	controller::instance().add_reload_ini_file_value_function(
		std::bind(&wheel_odometry::init, this)
	);
}

void wheel_odometry::init() {
	for (auto&& i : _lpf_dataset) {
		if (_is_lpf_enable) {
			i.second.set(
				ini_parser::instance().get<float>("setting", "encoder_" + std::to_string(i.first) + "_lpf_p")
			);
		} else {
			i.second.set(0.0f);
		}
	}
}

void wheel_odometry::enable_lpf() {
	if (!_is_lpf_enable) {
		_is_lpf_enable = true;
		init();
	}
	_is_lpf_enable = true;
	init();
}

void wheel_odometry::disable_lpf() {
	if (_is_lpf_enable) {
		_is_lpf_enable = false;
		init();
	}
	_is_lpf_enable = false;
	init();
}

float wheel_odometry::get_tire_frequency_kHz(int index) {
	//float coeff = ini_parser::instance().get<float>("");
	return get_raw(index) / ini_parser::instance().get<int>("encoder_profile", "encoder_resolution");
}

float wheel_odometry::get_tire_frequency_Hz(int index) {
	return get_tire_frequency_kHz(index) * 1000.0f;
}

float wheel_odometry::get_tire_advanced_speed_cm_per_sec(int index) {
	float circumference_cm = 2.0f * M_PI;
	circumference_cm *= ini_parser::instance().get<float>("omni_wheel", "tire_radius_cm");
	return get_tire_frequency_Hz(index) * circumference_cm;
}

float wheel_odometry::get_raw(int index) {
	int id = serial_connected_mcu::ENCODER_SPEED1 + index;
	float coeff = ini_parser::instance().get<float>("encoder_profile", "encoder_coeff" + std::to_string(index));
	float raw = coeff * serial_connected_mcu::serial_connected_mcu_master::instance().get_raw(id);
	return _lpf_dataset.at(index).update(raw);
}

float wheel_odometry::get_heading_rad() {
	return 0.0f;
}
