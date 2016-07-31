#include <robot/arm.hpp>
#include <ini_parser.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <controller/controller.hpp>
#include <i2c/i2c.hpp>

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

	/*
	i2c::instance().set("length", _length_pid.update(controller::instance().get("length")));
	i2c::instance().set("width",  _length_pid.update(controller::instance().get("width")));
	*/

	float pos[3];
	pos[0] = serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::POTENTIONMETER1);
	pos[1] = serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::POTENTIONMETER2);
	pos[2] = serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::POTENTIONMETER3);

	i2c::instance().set("length", controller::instance().get("length") - pos[0]);
	i2c::instance().set("height",  controller::instance().get("height") - pos[1]);
	i2c::instance().set("width",  controller::instance().get("width") - pos[2]);
}

void arm::update_angle() {
	float angle = controller::instance().get("angle");
	serial_connected_mcu::serial_connected_mcu_master::instance().set(serial_connected_mcu::ESC1,  angle);
	serial_connected_mcu::serial_connected_mcu_master::instance().set(serial_connected_mcu::ESC2, -angle);
}
