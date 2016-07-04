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

class moving_object {
public:
	moving_object();
	~moving_object();

	void update();

private:
	static const float _angular_velocity_coefficient;
	omni_wheel _omni_wheel;
	basic_low_pass_filter _lpf_velocity_x;
	basic_low_pass_filter _lpf_velocity_y;
	basic_low_pass_filter _lpf_angular_velocity;
};

#endif /* MOVING_OBJECT_HPP_ */
