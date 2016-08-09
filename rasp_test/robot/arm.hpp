/*
 * arm.hpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#ifndef ARM_HPP_
#define ARM_HPP_

#include <pid/speed_pid.hpp>

class arm {
public:
	arm();

	void update();

private:
	void update_angle();

	float _width;
	float _length;

	speed_pid<float> _length_pid;
	speed_pid<float> _width_pid;
	speed_pid<float> _height_pid;
};

#endif /* ARM_HPP_ */
