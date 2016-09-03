/*
 * speed_pid_impl.hpp
 *
 *  Created on: 2016/07/04
 *      Author: tomoya
 */

#ifndef PID_SPEED_PID_IMPL_HPP_
#define PID_SPEED_PID_IMPL_HPP_

#include <cmath>

template <class T>
speed_pid<T>::speed_pid(const T& kp, const T& ki, const T& kd) :	pid<T>(kp, ki, kd),
																		_J(T()),
																		_prev_error(T()) {}

template <class T>
speed_pid<T>::~speed_pid() {}

template <class T>
T speed_pid<T>::update(const T& e) {
	// todo : implement ini file
	float threshold = 0.01f;
	if (std::fabs(e) < threshold) {
		init();
		return pid<T>::_mv;
	}

	pid<T>::_mv += _F1 * e + _J;
	_J = _F2 * e + pid<T>::_kd * _prev_error;

	_prev_error = e;
	return pid<T>::_mv;
}

template <class T>
void speed_pid<T>::init() {
	pid<T>::_mv = T();
	_J = T();
	_prev_error = T();
}

template <class T>
void speed_pid<T>::update_specific_coeff() {
	_F1 = pid<T>::_kp + pid<T>::_ki / 2 + pid<T>::_kd;
	_F2 = -pid<T>::_kp + pid<T>::_ki / 2 - 2 * pid<T>::_kd;
}

#endif
