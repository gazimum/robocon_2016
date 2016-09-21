/*
 * todo:コントローラ遷移の時におそらくarm_length, arm_width, arm_height, arm_angleに0が命令される問題を解決する
*/

#include <thread>
#include <communication.hpp>
#include <config.hpp>
#include <robot/robot.hpp>
#include <i2c/i2c.hpp>
#include <controller/controller.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <potentiometer.hpp>

#include <iostream>

int main() {
	std::thread com_thread(std::ref(communication::instance()));

	try {
		while (true) {	// 通信スレッドがconfigの使用を終了したらメインスレッドの処理を開始する
			config::_mutex.lock();
			if (config::_is_usable_for_main_thread) {
				config::_mutex.unlock();
				break;
			}
			config::_mutex.unlock();
		}

		serial_connected_mcu::serial_connected_mcu_master::instance().init();

		while (!robot::instance().is_end()) {
			potentiometer::instance().update();
			robot::instance().update();
			i2c::instance().write();
			controller::instance().update();
			serial_connected_mcu::serial_connected_mcu_master::instance().communicate();
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		throw;
	}

	com_thread.join();

	return 0;
}
