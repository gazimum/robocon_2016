/*
 * servo_impl.hpp

 *
 *  Created on: 2016/07/08
 *      Author: tomoya
 */

#ifndef SERVO_IMPL_HPP_
#define SERVO_IMPL_HPP_

#include <servo.hpp>
#include <algorithm>

template <class T>
servo<T>::servo(float kp, float ki, float kd) : _position(float()),
												_pid(kp, ki, kd) {}

template <class T>
float servo<T>::operator()(float p) {
	set(p);
}

template <class T>
float servo<T>::set(float p) {
	_position = _pid(std::max(-1.0f, std::min(p, 1.0f)));
	// insert something to set new data.
	return _position;
}

template <class T>
float servo<T>::get() const {
	return _position;
}

#endif
