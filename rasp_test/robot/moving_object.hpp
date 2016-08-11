/*
 * moving_object.hpp
 *
 *  Created on: 2016/06/27
 *      Author: tomoya
 */

#ifndef MOVING_OBJECT_HPP_
#define MOVING_OBJECT_HPP_

#include <pid/position_pid.hpp>
#include "omni_wheel.hpp"
#include "lpf.hpp"

class moving_object {
public:
	moving_object();
	~moving_object();

	void update();

private:
	omni_wheel _omni_wheel;
	lpf<float> _lpf_velocity_x;
	lpf<float> _lpf_velocity_y;
	lpf<float> _lpf_angular_velocity;
};

#endif /* MOVING_OBJECT_HPP_ */
