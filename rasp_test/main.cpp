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

void main_thread_func() {
	std::this_thread::sleep_for(std::chrono::milliseconds(80));

	while (true) {
		robot::instance().update();
		i2c::instance().write();
		controller::instance().update();
	}
}

int main() {
	std::thread com_thread(std::ref(communication::instance()));
	std::thread main_thread(main_thread_func);

	com_thread.join();
	main_thread.join();

	return 0;
}
