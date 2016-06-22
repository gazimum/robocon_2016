/*
 * communication.cpp
 *
 *  Created on: 2016/06/21
 *      Author: tomoya
 */

#include <iostream>
#include <thread>
#include <chrono>
#include "communication.hpp"

communication::communication() {}

void communication::operator()(){
	while (true) {
		std::cout << "communication::operator()()" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

