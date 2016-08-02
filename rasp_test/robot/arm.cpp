#include <robot/arm.hpp>
#include <ini_parser.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <controller/controller.hpp>
#include <i2c/i2c.hpp>

#include <iostream>
#include <array>

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
			),
			_height_pid(
				ini_parser::instance().setting<float>("arm_height_pid_kp"),
				ini_parser::instance().setting<float>("arm_height_pid_ki"),
				ini_parser::instance().setting<float>("arm_height_pid_kd")
			) {}

void arm::update() {
	update_angle();

	/*
	i2c::instance().set("length", _length_pid.update(controller::instance().get("length")));
	i2c::instance().set("width",  _length_pid.update(controller::instance().get("width")));
	*/
	float pos[]{
		serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::POTENTIONMETER1),
		serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::POTENTIONMETER2),
		serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::POTENTIONMETER3)
		//0.0f,
		//0.0f,
		//0.0f
	};
	std::array<std::string, 3> name = {
			"length",
			"height",
			"width"
	};/*
	for (size_t i = 0; i < name.size(); ++i) {
		//std::cout << pos[i] << " ";

		float err = controller::instance().get(name[i]) - pos[i];
		i2c::instance().set(name[i], err * 2.5f);
	}
	*/

	float err = controller::instance().get("length");
	i2c::instance().set("length", err * 2.5f);

	err = controller::instance().get("height");// - pos[1];
	i2c::instance().set("height", err * 2.5f);

	err = controller::instance().get("width");
	i2c::instance().set("width", err * 2.5f);

	//std::cout << std::endl;
}

void arm::update_angle() {
	float angle = controller::instance().get("angle");
	serial_connected_mcu::serial_connected_mcu_master::instance().set(serial_connected_mcu::ESC1,  angle);
	serial_connected_mcu::serial_connected_mcu_master::instance().set(serial_connected_mcu::ESC2, -angle);
}
