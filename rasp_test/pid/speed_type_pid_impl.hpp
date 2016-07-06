/*
 * speed_type_pid_impl.hpp
 *
 *  Created on: 2016/07/04
 *      Author: tomoya
 */

#ifndef PID_SPEED_TYPE_PID_IMPL_HPP_
#define PID_SPEED_TYPE_PID_IMPL_HPP_

#include <pid/speed_type_pid.hpp>

template <class T>
speed_type_pid<T>::speed_type_pid() : _F1(_kp + _ki / 2 + _kd),
									  _F2(-_kp + _ki / 2 - 2 * _kd),
									  _J(T()),
									  _mv(T()),
									  _prev_error(T()){}

template <class T>
speed_type_pid<T>::~speed_type_pid() {}

template <class T>
T speed_type_pid<T>::update(const T& e) {
	_mv += _F1 * e + _J;
	j = _F2 * e + _kd * _prev_error;
	_prev_error = e;
}

#endif /*  */
