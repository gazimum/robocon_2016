/*
 * todo:PID制御時に時計回りに旋回するときエンコーダの値が段階的でないために起こる振動の原因究明と対策．
 * 	     反時計回りに旋回するときはエンコーダの値が段階的に変化し、なめらかな制御ができていることを確認済み．
 * 	     また、簡易コンで回したときは時計回り、反時計回り共に段階的に見えたことから、
 * 	     時計回り時はLPFによって段階的な値のように見えているだけであると推測できる．
 * 	     よって、反時計回り時にLPFが有効になっていない可能性があるため確認が必要である．
 * todo:微調整モードのティーチング機能を実験
 * todo:微調整モードに範囲制限を追加：設定ファイルに制約式を追加する形が理想．
 * 		 ↑ではなく下限値と上限値を設定ファイルに書く形が妥当であると考えられる．
 */

#include <thread>
#include <communication.hpp>
#include <robot/robot.hpp>
#include <i2c/i2c.hpp>
#include <controller/controller.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>

#include <iostream>

void main_thread_func() {
	using namespace serial_connected_mcu;

	try {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		serial_connected_mcu_master::instance().init();

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
}

int main() {
	std::thread com_thread(std::ref(communication::instance()));
	std::thread main_thread(main_thread_func);

	com_thread.join();
	main_thread.join();

	return 0;
}
