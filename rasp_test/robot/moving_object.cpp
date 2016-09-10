/*
 * moving_object.cpp
 *
 *  Created on: 2016/06/27
 *      Author: tomoya
 */

#include <ini_parser.hpp>
#include <map>
#include <cmath>
#include <string>
#include <robot/moving_object.hpp>
#include <robot/omni_wheel.hpp>
#include <server_shared_data.hpp>
#include <communication.hpp>
#include <controller/controller.hpp>
#include <pid/position_pid.hpp>
#include <robot/wheel_odometry.hpp>

moving_object::moving_object() {
	_command_lpf.insert(
			std::make_pair("velocity_x", lpf<float>())
	);
	_command_lpf.insert(
			std::make_pair("velocity_y", lpf<float>())
	);
	_command_lpf.insert(
			std::make_pair("angular_velocity", lpf<float>())
	);
	init_lpf();
	controller::instance().add_reload_ini_file_value_function(
			std::bind(&moving_object::init_lpf, this)
	);
}

moving_object::~moving_object() {}

void moving_object::update() {
	float vx = controller::instance().get("velocity_x");
	float vy = controller::instance().get("velocity_y");
	float av = controller::instance().get("angular_velocity");

	// 移動する状態だったらLPFを有効化
	float l = sqrt(vx * vx + vy * vy);
	float threshold = ini_parser::instance().get<float>("lpf", "lpf_enable_threshold_velocity");
	bool is_enable_lpf = (l > threshold);
	threshold = ini_parser::instance().get<float>("lpf", "lpf_enable_threshold_angular_velocity");
	is_enable_lpf = is_enable_lpf || (std::fabs(av) > threshold);
	if (is_enable_lpf) {
		enable_lpf();
	} else {
		// 移動する状態でないならLPFを無効化
		disable_lpf();
	}

	vx = _command_lpf.at("velocity_x").update(vx);
	vy = _command_lpf.at("velocity_y").update(vy);
	av = _command_lpf.at("angular_velocity").update(av);

	_omni_wheel.set_velocity(vx, vy);
	_omni_wheel.set_angular_velocity(av);
	_omni_wheel.write();
}

void moving_object::enable_lpf() {
	init_lpf();
}

void moving_object::disable_lpf() {
	for (auto&& i : _command_lpf) {
		i.second.set(0.0f);
	}
}

void moving_object::init_lpf() {
	for (auto&& i : _command_lpf) {
		i.second.set(
			ini_parser::instance().get<float>("lpf", i.first + "_lpf_p")
		);
	}
}
