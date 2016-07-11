#include <robot/arm.hpp>
#include <ini_parser.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>

#include <iostream>

arm::arm() : _length_servo(
				serial_connected_mcu::ESC1,
				ini_parser::instance().setting<float>("arm_length_pid_kp"),
				ini_parser::instance().setting<float>("arm_length_pid_ki"),
				ini_parser::instance().setting<float>("arm_length_pid_kd")
			),
			_angle_servo(
				serial_connected_mcu::ESC2,
				ini_parser::instance().setting<float>("arm_angle_pid_kp"),
				ini_parser::instance().setting<float>("arm_angle_pid_ki"),
				ini_parser::instance().setting<float>("arm_angle_pid_kd")
			),
			_width_servo(
				serial_connected_mcu::ESC3,
				ini_parser::instance().setting<float>("arm_width_pid_kp"),
				ini_parser::instance().setting<float>("arm_width_pid_ki"),
				ini_parser::instance().setting<float>("arm_width_pid_kd")
			) {}

void arm::update() {
	int16_t p = serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::POTENTIONMETER1);
	std::cout <<
		//serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::ENCODER1)
		p
	<< std::endl;
	serial_connected_mcu::serial_connected_mcu_master::instance().set(serial_connected_mcu::ESC1, p);
}
