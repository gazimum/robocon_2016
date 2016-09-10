/*
 * omniwheel.h
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#ifndef OMNI_WHEEL_HPP_
#define OMNI_WHEEL_HPP_

#include <pid/position_pid.hpp>

class wheel_odometry;

class omni_wheel {
public:
	omni_wheel();
	~omni_wheel();

	void write();
	void set_velocity(float x, float y);
	void set_angular_velocity(float v);
	void set_target_heading_rad(float heading_rad);

private:
	float _velocity_x;
	float _velocity_y;
	float _angular_velocity;
	float _target_heading_rad;
};

#endif /* OMNI_WHEEL_HPP_ */
