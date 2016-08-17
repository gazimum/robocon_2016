/*
 * position_pid_impl.hpp
 *
 *  Created on: 2016/08/08
 *      Author: tomoya
 */

#ifndef PID_POSITION_PID_IMPL_HPP_
#define PID_POSITION_PID_IMPL_HPP_

#include <cmath>

template <class T>
position_pid<T>::position_pid(const T& kp, const T& ki, const T& kd) :	pid<T>(kp, ki, kd),
																				_prev_error(T()),
																				_accum_error(T()) {}

template <class T>
position_pid<T>::~position_pid() {}

template <class T>
T position_pid<T>::update(const T& e) {
	// todo : implement ini file
	const float threshold = 0.01f;
	if (std::abs(e) < threshold) {
		init();
		return pid<T>::_mv;
	}

	pid<T>::_mv  = pid<T>::_kp * e;
	pid<T>::_mv += pid<T>::_ki * (e - _prev_error);
	pid<T>::_mv += pid<T>::_kd * _accum_error;

	_prev_error = e;
	_accum_error += e;
	return pid<T>::_mv;
}

template <class T>
void position_pid<T>::init() {
	pid<T>::_mv = T();
	_prev_error = T();
	_accum_error = T();
}

template <class T>
void position_pid<T>::update_specific_coeff() {}

#endif /* PID_POSITION_PID_IMPL_HPP_ */
