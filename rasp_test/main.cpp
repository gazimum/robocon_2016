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
#include <controller.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>

void main_thread_func() {
	using namespace serial_connected_mcu;
	std::this_thread::sleep_for(std::chrono::milliseconds(80));

	int16_t init_data[SIZE_OF_WRITE_DATA] {
		0, // ESC1
		0, // ESC2
		0  // ESC3
	};

	// 書き込みデータ初期化して書きこむ
	for (size_t i = 0; i < SIZE_OF_WRITE_DATA; ++i) {
		serial_connected_mcu_master::instance().set(i, init_data[i]);
	}
	serial_connected_mcu_master::instance().communicate();

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
