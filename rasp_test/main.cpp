/*
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB
#define BOOST_ERROR_CODE_HEADER_ONLY
#define BOOST_SYSTEM_NO_LIB
*/

#include <thread>
#include <communication.hpp>
#include <robot/robot.hpp>
#include <i2c/i2c.hpp>
#include <controller/controller.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>

//#include <iostream>
//#include <pid/speed_type_pid.hpp>
// serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::ENCODER1 + i)
// i2c::instance().set("wheel" + std::to_string(i), p[i]);

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
