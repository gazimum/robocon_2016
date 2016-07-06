/*
 * pid.cpp
 *
 *  Created on: 2016/07/04
 *      Author: tomoya
 */

#include <pid.hpp>

template <class T>
pid<T>::pid(const T& kp, const T& ki, const T& kd, const T& t) : _kp(kp),
																 _ki(ki),
																 _kd(kd),
																 _T(t) {}

template <class T>
void pid<T>::operator()(const T& e) {
	update(e);
}

template <class T>
void pid<T>::operator()(const T& e) {
	update(e);
}

template <class T>
void pid<T>::update(T&& e) {
	operator()(e);
}

