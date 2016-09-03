/*
 * omniwheel.h
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#ifndef OMNI_WHEEL_HPP_
#define OMNI_WHEEL_HPP_

#include <boost/numeric/ublas/vector.hpp>
#include <pid/speed_pid.hpp>

class wheel_odometry;

class omni_wheel {
public:
	omni_wheel();
	~omni_wheel();

	void write();
	void set_velocity(float x, float y);
	void set_angular_velocity(float v);

	static constexpr size_t _wheel_num = 3;

private:
	void update_tire_frequency_pid_coeff();

	float _velocity_x;
	float _velocity_y;
	float _angular_velocity;
	wheel_odometry* _wheel_odometry;

	speed_pid<float> _tire_frequency_pid[_wheel_num];
};

#endif /* OMNI_WHEEL_HPP_ */
