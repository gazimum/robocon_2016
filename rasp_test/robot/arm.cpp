/*
 * arm.cpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#include "arm.hpp"
#include "server_shared_data.hpp"
#include <controller.hpp>
#include <ini_parser.hpp>
#include <servo.hpp>

arm::arm() : _length(float()),
			 _angle(float()),
			 _width(float()),
			 _length_servo(
					 ini_parser::instance().setting<float>("arm_length_pid_kp"),
					 ini_parser::instance().setting<float>("arm_length_pid_ki"),
					 ini_parser::instance().setting<float>("arm_length_pid_kd")
			),
			 _angle_servo(
					 ini_parser::instance().setting<float>("arm_angle_pid_kp"),
					 ini_parser::instance().setting<float>("arm_angle_pid_ki"),
					 ini_parser::instance().setting<float>("arm_angle_pid_kd")
			),
			 _width_servo(
					 ini_parser::instance().setting<float>("arm_width_pid_kp"),
					 ini_parser::instance().setting<float>("arm_width_pid_ki"),
					 ini_parser::instance().setting<float>("arm_width_pid_kd")) {}

void arm::update() {
	_length += controller::instance().get("lengther");
	_angle += controller::instance().get("angle_adjuster");
	_width += controller::instance().get("widener");

	/*
	_controller_map["grab"] = controller[config.key_config("grab")];
	_controller_map["lengther"] = controller[config.key_config("lengther")];
	_controller_map["widener"] = controller[config.key_config("widener")];
	_controller_map["height_adjuster"] = controller[config.key_config("height_adjuster")];
	*/
}
