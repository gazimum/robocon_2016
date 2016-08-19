/*
 * robot.hpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#ifndef ROBOT_HPP_
#define ROBOT_HPP_

#include "moving_object.hpp"
#include "arm.hpp"
#include "singleton.hpp"

class robot : public singleton<robot> {
public:
	void update();

	bool is_end();

private:
	friend class singleton<robot>;
	robot();

	moving_object _moving_object;
	arm _arm;
};

#endif /* ROBOT_HPP_ */
