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
#include "i2c_profile.hpp"

#include <iostream>

i2c::i2c() {
	for (const auto& i : i2c_device_name) {
		_filehandles[i] = wiringPiI2CSetup(i2c_address.at(i));
	}
}

void i2c::set(std::string name, float p) {
	_buffers[name] = std::max(-1.0f , std::min(p, 1.0f));
}

void i2c::write() {
	for (const auto& i : i2c_device_name) {
		char buff = abs(_buffers[i] * 100.0f);

		if (_buffers[i] < 0.0f) {
			buff |= 0x80;
		}

		wiringPiI2CWrite(_filehandles[i], buff);
	}
}

