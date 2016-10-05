/*
 * moving_object.cpp
 *
 *  Created on: 2016/06/27
 *      Author: tomoya
 */

#include <map>
#include <cmath>
#include <string>
#include <robot/moving_object.hpp>
#include <robot/omni_wheel.hpp>
#include <server_shared_data.hpp>
#include <communication.hpp>
#include <config.hpp>
#include <controller/controller.hpp>
#include <robot/wheel_odometry.hpp>
#include <lpf/lpf_manager.hpp>
#include <lpf/lpf.hpp>

const std::vector<std::string> moving_object::_lpf_name_dataset {
	"velocity_x",
	"velocity_y",
	"angular_velocity"
};

moving_object::moving_object() : _velocity_x(float()),
									  _velocity_y(float()),
									  _angular_velocity(float()),
									  _target_heading_rad(float()){
	for (const auto& i : _lpf_name_dataset) {
		lpf_manager<float>::instance().add<lpf<float>>(i);
	}
}

moving_object::~moving_object() {}

void moving_object::update() {
	float ability_threshold = config::instance().get<float>("setting", "ability_enable_threshold");
	if (controller::instance().get("is_enable_moving_object") < ability_threshold) {
		return;
	}

	float l = sqrt(_velocity_x * _velocity_x + _velocity_y * _velocity_y);
	float threshold = config::instance().get<float>("lpf", "lpf_enable_threshold_velocity");
	if (l > threshold) {
		_velocity_x = lpf_manager<float>::instance().get("velocity_x").update(_velocity_x);
		_velocity_y = lpf_manager<float>::instance().get("velocity_y").update(_velocity_y);
	} else {
		lpf_manager<float>::instance().get("velocity_x").init();
		lpf_manager<float>::instance().get("velocity_y").init();
	}

	threshold = config::instance().get<float>("lpf", "lpf_enable_threshold_angular_velocity");
	if (std::fabs(_angular_velocity) > threshold) {
		_angular_velocity = lpf_manager<float>::instance().get("angular_velocity").update(_angular_velocity);
	} else {
		lpf_manager<float>::instance().get("angular_velocity").init();
	}
	_target_heading_rad += _angular_velocity;

	write();
}

void moving_object::set_velocity(float vx, float vy) {
	_velocity_x = vx;
	_velocity_y = vy;
}

void moving_object::set_angular_velocity(float av) {
	_angular_velocity = av;
}
void moving_object::set_target_heading_rad(float heading_rad) {
	_target_heading_rad = heading_rad;
}
