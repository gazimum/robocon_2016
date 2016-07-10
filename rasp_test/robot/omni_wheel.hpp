/*
 * omniwheel.h
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#ifndef OMNI_WHEEL_HPP_
#define OMNI_WHEEL_HPP_

#include <boost/numeric/ublas/vector.hpp>

class omni_wheel {
public:
	omni_wheel();
	~omni_wheel();

	void write();
	void set_velocity(float x, float y);
	void set_angular_velocity(float v);

private:
	static constexpr size_t _wheel_num = 3;
	static const float _wheel_directions_x[_wheel_num];
	static const float _wheel_directions_y[_wheel_num];
	static const float _wheel_position_angles[_wheel_num];

	float _velocity_x;
	float _velocity_y;
	float _angular_velocity;
};

#endif /* OMNI_WHEEL_HPP_ */
