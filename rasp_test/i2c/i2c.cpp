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
#include <string>

const size_t i2c::_i2c_try_num = 10;

i2c::i2c() : _i2c_device_num(ini_parser::instance().get<size_t>("i2c_profile", "i2c_device_num")) {
	for (size_t i = 0; i < _i2c_device_num; ++i) {
		std::string name = ini_parser::instance().get<std::string>("i2c_profile", "i2c_device_name" + std::to_string(i));
		int address = ini_parser::instance().get<int>("i2c_profile", "i2c_address" + std::to_string(i));
		_filehandles[name] = wiringPiI2CSetup(address);
	}
}

void i2c::set(std::string name, int p) {
	_write_dataset[name] = p;
}

void i2c::write() {
	for (size_t i = 0; i < _i2c_device_num; ++i) {
		std::string name = ini_parser::instance().get<std::string>("i2c_profile", "i2c_device_name" + std::to_string(i));

		size_t try_num = 1;
		while (wiringPiI2CWrite(_filehandles[name], _write_dataset[name]) != 0) {
			if (try_num++ > ini_parser::instance().get<int>("i2c_profile", "i2c_try_num")) {
				std::string err = "error : I2C bus to the \"" + name + "\" is not working now.\n";
				err += "(hint : Rebooting all I2C device might be a good way to recover the I2C bus.)\n";
				err += "(hint : Perhaps you mistook I2C device address.)";
				throw err;
			}

			int address = ini_parser::instance().get<int>("i2c_profile", "i2c_address" + std::to_string(i));
			_filehandles[name] = wiringPiI2CSetup(address);
		}
	}
}

