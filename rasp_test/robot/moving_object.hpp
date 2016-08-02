/*
 * moving_object.hpp
 *
 *  Created on: 2016/06/27
 *      Author: tomoya
 */

#ifndef MOVING_OBJECT_HPP_
#define MOVING_OBJECT_HPP_

#include "omni_wheel.hpp"
#include "lpf.hpp"
#include <pid/speed_type_pid.hpp>

class moving_object {
public:
	moving_object();
	~moving_object();

	void update();

private:
	omni_wheel _omni_wheel;
	lpf<8, float> _lpf_velocity_x;
	lpf<8, float> _lpf_velocity_y;
	lpf<8, float> _lpf_angular_velocity;
	speed_type_pid<float> _pid_velocity_x;
	speed_type_pid<float> _pid_velocity_y;
};

#endif /* MOVING_OBJECT_HPP_ */
