#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB
#define BOOST_ERROR_CODE_HEADER_ONLY
#define BOOST_SYSTEM_NO_LIB

#include <thread>
#include "communication.hpp"
#include "robot.hpp"

int main() {
	std::thread com_thread(std::ref(communication::instance()));
	robot robo;

	while (true) {
		robo.update();
	}

	return 0;
}
