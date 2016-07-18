/*
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB
#define BOOST_ERROR_CODE_HEADER_ONLY
#define BOOST_SYSTEM_NO_LIB
*/

// todo : add ini file
// config.setting<int>("arm_abilities_position_num")
// config.setting<int>("arm_abilities_position_num")
// config.key_config<std::string>("arm_abilities_position_index_" + std::to_string(i))
// ini_parser::instance().setting<float>("normalization_coeff_" + i.first)
// "command_coeff_" + i.first

#include <thread>
#include <communication.hpp>
#include <robot/robot.hpp>
#include <i2c/i2c.hpp>
#include <controller/controller.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>

void main_thread_func() {
	using namespace serial_connected_mcu;
	std::this_thread::sleep_for(std::chrono::milliseconds(80));

	serial_connected_mcu_master::instance().init();

	while (true) {
		robot::instance().update();
		i2c::instance().write();
		controller::instance().update();
		serial_connected_mcu::serial_connected_mcu_master::instance().communicate();
	}
}

int main() {
	std::thread com_thread(std::ref(communication::instance()));
	std::thread main_thread(main_thread_func);

	com_thread.join();
	main_thread.join();

	return 0;
}
