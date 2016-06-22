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
