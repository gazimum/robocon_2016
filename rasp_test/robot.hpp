/*
 * robot.hpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#ifndef ROBOT_HPP_
#define ROBOT_HPP_

#include "omni_wheel.hpp"
#include "arm.hpp"

class robot {
public:
	robot();
	~robot();

private:
	omni_wheel _omni_wheel;
	arm _arm;
};

#endif /* ROBOT_HPP_ */
