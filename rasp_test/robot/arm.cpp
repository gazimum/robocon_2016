#include <robot/arm.hpp>
#include <ini_parser.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <controller/controller.hpp>

#include <iostream>

arm::arm() : _width(0.0f),
			 _length(0.0f),
			 _length_pid(
				ini_parser::instance().setting<float>("arm_length_pid_kp"),
				ini_parser::instance().setting<float>("arm_length_pid_ki"),
				ini_parser::instance().setting<float>("arm_length_pid_kd")
			),
			_width_pid(
				ini_parser::instance().setting<float>("arm_width_pid_kp"),
				ini_parser::instance().setting<float>("arm_width_pid_ki"),
				ini_parser::instance().setting<float>("arm_width_pid_kd")
			) {}

void arm::update() {
	update_angle();
	_length_pid.update(controller::instance().get("lengther"));
	_width_pid.update(controller::instance().get("widener"));
}

void arm::update_angle() {
	float angle = controller::instance().get("angle_adjuster");
	serial_connected_mcu::serial_connected_mcu_master::instance().set(serial_connected_mcu::ESC1, angle);
	serial_connected_mcu::serial_connected_mcu_master::instance().set(serial_connected_mcu::ESC2, 1.0f - angle);
}
