/*
 * arm.hpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#ifndef ARM_HPP_
#define ARM_HPP_

#include <servo.hpp>

class arm {
public:
	arm();

	void update();

private:
	float _length;
	float _angle;
	float _width;

	basic_servo _length_servo;
	basic_servo _angle_servo;
	basic_servo _width_servo;
};

#endif /* ARM_HPP_ */
