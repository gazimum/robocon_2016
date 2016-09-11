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
#include <controller/controller.hpp>
#include <robot/wheel_odometry.hpp>
#include <ini_parser.hpp>
#include <lpf/lpf_manager.hpp>
#include <lpf/lpf.hpp>

const std::string moving_object::_lpf_name_dataset {
	"velocity_x",
	"velocity_y",
	"angular_velocity"
};

moving_object::moving_object() {
	for (const auto& i : _lpf_name_dataset) {
		lpf_manager<float>::instance().add<lpf<float>>(i);
	}
}

moving_object::~moving_object() {}

void moving_object::update() {
	float vx = controller::instance().get("velocity_x");
	float vy = controller::instance().get("velocity_y");
	float av = controller::instance().get("angular_velocity");

	float l = sqrt(vx * vx + vy * vy);
	float threshold = ini_parser::instance().get<float>("lpf", "lpf_enable_threshold_velocity");
	bool is_enable_lpf = (l > threshold);
	threshold = ini_parser::instance().get<float>("lpf", "lpf_enable_threshold_angular_velocity");
	is_enable_lpf = is_enable_lpf || (std::fabs(av) > threshold);
	if (is_enable_lpf) {
		vx = lpf_manager<float>::instance().get("velocity_x").update(vx);
		vy = lpf_manager<float>::instance().get("velocity_y").update(vy);
		av = lpf_manager<float>::instance().get("angular_velocity").update(av);
	} else {
		for (const auto& i : _lpf_name_dataset) {
			lpf_manager<float>::instance().get(i).init();
		}
	}


	_omni_wheel.set_velocity(vx, vy);
	_omni_wheel.set_angular_velocity(av);
	_omni_wheel.write();
}
