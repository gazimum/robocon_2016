/*
 * todo:PIDの係数をarm_stateごとに切り替える．
 * todo:起動時に設定ファイルのバックアップをとる機能を付ける．
 */

#include <thread>
#include <communication.hpp>
#include <robot/robot.hpp>
#include <i2c/i2c.hpp>
#include <controller/controller.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <ini_parser.hpp>
#include <potentiometer.hpp>

#include <iostream>

int main() {
	std::thread com_thread(std::ref(communication::instance()));

	try {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		while (true) {	// 通信スレッドがini_parserの使用を終了したらメインスレッドの処理を開始する
			ini_parser::_mutex.lock();
			if (ini_parser::_is_usable_for_main_thread) {
				ini_parser::_mutex.unlock();
				break;
			}
			ini_parser::_mutex.unlock();
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
