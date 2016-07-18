#include <robot/arm.hpp>
#include <ini_parser.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <controller/controller.hpp>

#include <iostream>

arm::arm() : _width(0.5f),
			 _length(0.5f),
			 _angle(M_PI / 2.0f),
			 _length_servo(
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
	/*
	_controller_map["grab"] = controller[config.key_config<std::string>("grab")];
	_controller_map["lengther"] = controller[config.key_config<std::string>("lengther")];
	_controller_map["widener"] = controller[config.key_config<std::string>("widener")];
	 */
	_angle += controller::instance().get("angle_adjuster");
	serial_connected_mcu::serial_connected_mcu_master::instance().set(serial_connected_mcu::ESC1, _angle);
	serial_connected_mcu::serial_connected_mcu_master::instance().set(serial_connected_mcu::ESC2, 1.0f - _angle);

	/*
	int16_t p[] = {
			serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::POTENTIONMETER1),
			serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::POTENTIONMETER2),
			serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::POTENTIONMETER3)
	};*/
	/*
	float e[] = {
			serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::ENCODER1),
			serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::ENCODER2),
			serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::ENCODER3)
	};*/
	/*
	std::cout <<
			e[0] << " " <<
			e[1] << " " <<
			e[2] << " " <<
		serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::ENCODER1) << " " <<
		serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::ENCODER2) << " " <<
		serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::ENCODER3) << " "
		p[0] << " " <<
		p[1] << " " <<
		p[2]
	<< std::endl;

	serial_connected_mcu::serial_connected_mcu_master::instance().set(serial_connected_mcu::ESC1, p[0]);
	serial_connected_mcu::serial_connected_mcu_master::instance().set(serial_connected_mcu::ESC2, p[1]);
	serial_connected_mcu::serial_connected_mcu_master::instance().set(serial_connected_mcu::ESC3, p[2]);
	*/
}
