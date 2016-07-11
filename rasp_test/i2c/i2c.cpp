/*
 * i2c.cpp
 *
 *  Created on: 2016/06/26
 *      Author: tomoya
 */

#include <algorithm>
#include <cstdlib>
#include <array>
#include <map>
#include <string>
#include <wiringPiI2C.h>
#include "i2c.hpp"
#include <ini_parser.hpp>

#include <iostream>

i2c::i2c() : _i2c_device_num(ini_parser::instance().i2c_profile<size_t>("i2c_device_num")) {
	for (size_t i = 0; i < _i2c_device_num; ++i) {
		std::string name = ini_parser::instance().i2c_profile<std::string>("i2c_device_name" + std::to_string(i));
		int address = ini_parser::instance().i2c_profile<int>("i2c_address" + std::to_string(i));
		_filehandles[name] =wiringPiI2CSetup(address);
	}
}

void i2c::set(std::string name, float p) {
	_buffers[name] = std::max(-1.0f , std::min(p, 1.0f));
}

void i2c::write() {
	for (size_t i = 0; i < _i2c_device_num; ++i) {
		std::string name = ini_parser::instance().i2c_profile<std::string>("i2c_device_name" + std::to_string(i));

		/*
		char buff = abs(_buffers[name] * 127.0f);

		if (_buffers[name] < 0.0f) {
			buff |= 0x80;
		}
		*/;
		wiringPiI2CWrite(_filehandles[name], std::round(_buffers[name] * 127.0f));
	}
}

