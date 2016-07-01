/*
 * i2c.cpp
 *
 *  Created on: 2016/06/26
 *      Author: tomoya
 */

#include <array>
#include <map>
#include <string>
#include <wiringPiI2C.h>
#include "i2c.hpp"
#include "i2c_profile.hpp"

i2c::i2c() {
	for (const auto& i : i2c_device_name) {
		_filehandles[i] = wiringPiI2CSetup(i2c_address.at(""));
	}
}

void i2c::set(std::string name, float p) {
	_buffers[name] = p;
}

void i2c::write() {
	for (size_t i = 0; i < i2c_device_num; ++i) {
		// wiringPiI2CWrite(_filehandles[i2c_device_name[i], _buffer[i2c_device_name[i]]);
	}
}

