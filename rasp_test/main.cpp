/*
 * todo:処理が正常に進まない問題．singleton	で死ぬのではなく．robot::instance()で死ぬ．
 * todo:angleに渡しているポテメのデータはLPFが効いているか？
 * todo:↑は効いていなかったため修正した．要テスト．
 * todo:コントローラ遷移時につかみ状態を維持するようにする．
 * todo:起動時に設定ファイルのバックアップをとる機能を付ける．
 * todo:flex_controllerの実装をしたため実際に操縦者に使ってもらってフィードバックを得る．
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
			std::cout << "main loop" << std::endl;
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

	std::cout << "main thread is end" << std::endl;
}

int main() {
	std::thread com_thread(std::ref(communication::instance()));
	std::thread main_thread(main_thread_func);

	com_thread.join();
	main_thread.join();

	return 0;
}
