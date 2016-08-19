#include <robot/arm.hpp>
#include <ini_parser.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <controller/controller.hpp>
#include <i2c/i2c.hpp>

#include <iostream>

arm::arm() {
	std::string name[] {
		"length",
		"width",
		"height"
	};

	for (const auto& i : name) {
		_pid.insert(pid_container_type::value_type(i, {
			ini_parser::instance().get<float>("pid_coeff", "arm_" + i + "_pid_kp"),
			ini_parser::instance().get<float>("pid_coeff", "arm_" + i + "_pid_ki"),
			ini_parser::instance().get<float>("pid_coeff", "arm_" + i + "_pid_kd")
		}));
	}

	update_pid_coeff();
}

void arm::update() {
	if (controller::instance().get("reload_ini_file") > 0.0f) {
		update_pid_coeff();
	}

	update_angle();

	float pos[]{
		serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::POTENTIONMETER1),
		serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::POTENTIONMETER2),
		serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::POTENTIONMETER3)
	};

	float err = controller::instance().get("length");
	i2c::instance().set("length", err * 2.5f);

	err = controller::instance().get("height") - pos[1];
	i2c::instance().set("height", _pid.at("height").update(err));

	err = controller::instance().get("width");
	i2c::instance().set("width", err * 2.5f);
}

void arm::update_angle() {
	float angle = controller::instance().get("angle");
	serial_connected_mcu::serial_connected_mcu_master::instance().set(serial_connected_mcu::ESC1,  angle);
	serial_connected_mcu::serial_connected_mcu_master::instance().set(serial_connected_mcu::ESC2, -angle);
}

void arm::update_pid_coeff() {
	for (auto&& i : _pid) {
		i.second.update_coeff(
			ini_parser::instance().get<float>("pid_coeff", "arm_" + i.first + "_pid_kp"),
			ini_parser::instance().get<float>("pid_coeff", "arm_" + i.first + "_pid_ki"),
			ini_parser::instance().get<float>("pid_coeff", "arm_" + i.first + "_pid_kd")
		);
	}
}
