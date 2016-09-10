/*
 * pid.cpp
 *
 *  Created on: 2016/07/04
 *      Author: tomoya
 */

#ifndef PID_IMPL_HPP_
#define PID_IMPL_HPP_

#include "pid.hpp"

#include <iostream>

template <class T>
pid<T>::pid(const T& kp, const T& ki, const T& kd) : _mv(T()) {
	update_coeff(kp, ki, kd);
}

template <class T>
T pid<T>::operator()(const T& e) {
	return update(e);
}

template <class T>
void pid<T>::update_coeff(float kp, float ki, float kd) {
	_kp = kp;
	_ki = ki;
	_kd = kd;

	update_specific_coeff();
}

#endif
