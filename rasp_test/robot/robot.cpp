/*
 * robot.cpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#include <robot/robot.hpp>
#include <robot/omni_wheel.hpp>
#include <controller/controller.hpp>

robot::robot() : _moving_object(new omni_wheel) {}

void robot::update() {
	_arm.update();
	update_moving_object();
}

void robot::update_moving_object() {
	_moving_object->set_velocity(
		controller::instance().get("velocity_x"),
		controller::instance().get("velocity_y")
	);
	_moving_object->set_angular_velocity(
		controller::instance().get("angular_velocity")
	);
	_moving_object->update();
}

bool robot::is_end() {
	return false;
}
