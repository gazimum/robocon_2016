/*
 * i2c.cpp
 *
 *  Created on: 2016/06/26
 *      Author: tomoya
 */

#include <array>
#include <map>
#include <string>
#include "i2c.hpp"
#include "i2c_profile.hpp"

i2c::i2c() {
	for (size_t i = 0; i < i2c_device_num; ++i) {
		_filehandles[i2c_device_name[i]] = 0; // wiringPiI2CSetup(i2c_address.at(i2c_device_name[i]));
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

