#include <robot/arm.hpp>
#include <ini_parser.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <controller/controller.hpp>
#include <dc_motor.hpp>
#include <solenoid_valve.hpp>

const std::vector<std::string> arm::_dc_motor_name_dataset {
	"length",
	"height"
};

const std::map<std::string, serial_connected_mcu::read_id> arm::_read_id_dataset {
	{"length", serial_connected_mcu::POTENTIONMETER1},
	{"height", serial_connected_mcu::POTENTIONMETER3}
};

const std::vector<std::string> arm::_solenoid_valve_name_dataset {
	"width",
	"lock"
};

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

	for (const auto& name : _dc_motor_name_dataset) {
		serial_connected_mcu::read_id id = _read_id_dataset.at(name);
		float position = serial_connected_mcu::serial_connected_mcu_master::instance().get(id);
		float target = controller::instance().get(name);
		float mv = _pid.at(name).update(target - position);
		dc_motor::instance().set(name, mv);
	}
	for (const auto& i : _solenoid_valve_name_dataset) {
		solenoid_valve::instance().set(i, controller::instance().get(i));
	}
}

void arm::update_angle() {
	serial_connected_mcu::serial_connected_mcu_master::instance().set(
		serial_connected_mcu::ESC1, controller::instance().get("angle_left")
	);
	serial_connected_mcu::serial_connected_mcu_master::instance().set(
		serial_connected_mcu::ESC2, controller::instance().get("angle_right")
	);
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
