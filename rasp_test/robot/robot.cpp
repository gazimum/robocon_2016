/*
 * robot.cpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#include "robot.hpp"
#include <potentiometer.hpp>

robot::robot() {}

void robot::update() {
	potentiometer::instance().update();
	_arm.update();
	_moving_object.update();
}

bool robot::is_end() {
	return false;
}
