/*
 * robot.cpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#include "robot.hpp"

robot::robot() {}

void robot::update() {
	_arm.update();
	_moving_object.update();
}

