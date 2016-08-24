/*
 * dc_motor.cpp
 *
 *  Created on: 2016/08/23
 *      Author: u
 */

#include <cmath>
#include <dc_motor.hpp>
#include <i2c/i2c.hpp>

void dc_motor::set(std::string name, float p) {
	p = std::max(-1.0f , std::min(p, 1.0f));
	i2c::instance().set(name, std::round(p * 127.0f));
}
