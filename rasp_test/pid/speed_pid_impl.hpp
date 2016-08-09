/*
 * speed_pid_impl.hpp
 *
 *  Created on: 2016/07/04
 *      Author: tomoya
 */

#ifndef PID_SPEED_PID_IMPL_HPP_
#define PID_SPEED_PID_IMPL_HPP_

#include <pid/pid.hpp>
#include <pid/speed_pid.hpp>

template <class T>
speed_pid<T>::speed_pid(const T& kp, const T& ki, const T& kd) :	pid<T>(kp, ki, kd),
																	_F1(kp + ki / 2 + kd),
																	_F2(-kp + ki / 2 - 2 * kd),
																	_J(T()),
																	_mv(T()),
																	_prev_error(T()) {}

template <class T>
speed_pid<T>::~speed_pid() {}

template <class T>
T speed_pid<T>::update(const T& e) {
	_mv += _F1 * e + _J;
	_J = _F2 * e + pid<T>::_kd * _prev_error;
	_prev_error = e;
	return _mv;
}

#endif
