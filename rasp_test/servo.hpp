/*
 * servo.hpp
 *
 *  Created on: 2016/07/08
 *      Author: tomoya
 */

#ifndef SERVO_HPP_
#define SERVO_HPP_

template <class T>
class servo {
public:
	servo(float kp, float ki, float kd);

	float operator()(float p);
	float set(float p);
	float get() const;

private:
	float _position;
	T _pid;
};

#include <servo_impl.hpp>

#include <pid/speed_type_pid.hpp>
typedef servo<speed_type_pid<float>> basic_servo;

#endif /* SERVO_HPP_ */
