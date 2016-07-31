/*
 * robot.cpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#include "robot.hpp"

#include <iostream>

robot::robot() {}

void robot::update() {
	_arm.update();
	_moving_object.update();
}

