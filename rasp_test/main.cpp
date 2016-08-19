#include <thread>
#include <communication.hpp>
#include <robot/robot.hpp>
#include <i2c/i2c.hpp>
#include <controller/controller.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>

void main_thread_func() {
	using namespace serial_connected_mcu;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	serial_connected_mcu_master::instance().init();

	while (!robot::instance().is_end()) {
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
