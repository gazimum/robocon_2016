#include <robot/arm.hpp>
#include <ini_parser.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <controller/controller.hpp>
#include <dc_motor.hpp>
#include <pid/pid_manager.hpp>
#include <solenoid_valve.hpp>
#include <potentiometer.hpp>

const std::vector<std::string> arm::_dc_motor_name_dataset {
	"arm_length",
	"arm_height"
};

const std::vector<std::string> arm::_solenoid_valve_name_dataset {
	"arm_width",
	"lock"
};

arm::arm() {
	for (const auto& i : _dc_motor_name_dataset) {
		pid_manager<float>::instance().add_pid<position_pid>(i);
	}
}

void arm::update() const {
	update_angle();

	for (const auto& name : _dc_motor_name_dataset) {
		float position = potentiometer::instance().get_position(name);
		float target = controller::instance().get(name);
		float mv = pid_manager<float>::instance().get_pid(name).update(target - position);
		dc_motor::instance().set(name, mv);

		std::cout << name << " " << target - position << ", ";
	}
	std::cout << std::endl;
	for (const auto& i : _solenoid_valve_name_dataset) {
		solenoid_valve::instance().set(i, controller::instance().get(i));
	}
}

void arm::update_angle() const {
	serial_connected_mcu::serial_connected_mcu_master::instance().set(
		serial_connected_mcu::ESC1,
		controller::instance().get("angle_left") + controller::instance().get("angle_base_left")
	);
	serial_connected_mcu::serial_connected_mcu_master::instance().set(
		serial_connected_mcu::ESC2,
		controller::instance().get("angle_right") + controller::instance().get("angle_base_right")
	);
}
