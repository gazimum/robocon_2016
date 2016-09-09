/*
 * todo:PIDの係数をarm_stateごとに切り替える．
 * todo:角度制御で操作中に急に動くことがある問題に対処する．
 * 			↑は係数の調整で改善できたように見える．要実験．
 * todo:起動時に設定ファイルのバックアップをとる機能を付ける．
 */

#include <thread>
#include <communication.hpp>
#include <robot/robot.hpp>
#include <i2c/i2c.hpp>
#include <controller/controller.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>

#include <iostream>

int main() {
	std::thread com_thread(std::ref(communication::instance()));

	try {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		serial_connected_mcu::serial_connected_mcu_master::instance().init();

		while (!robot::instance().is_end()) {
			robot::instance().update();
			i2c::instance().write();
			controller::instance().update();
			serial_connected_mcu::serial_connected_mcu_master::instance().communicate();
		}
	}
	catch (const char* s) {
		std::cout << s << std::endl;
	}
	catch (std::string& s) {
		std::cout << s << std::endl;
	}

	com_thread.join();

	return 0;
}
