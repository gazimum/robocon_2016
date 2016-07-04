/*
 * lpf.cpp
 *
 *  Created on: 2016/07/04
 *      Author: tomoya
 */

#include <lpf.hpp>

template <size_t P, class T>
lpf<P, T>::lpf(T n) : _value(n) {}

template <size_t P, class T>
T lpf<P, T>::operator()(const T& n) {
	return update(n);
}

template <size_t P, class T>
T lpf<P, T>::update(const T& n) {
	_value = _value * P + n * (100 - P);
	_value *= 0.01f;
	return _value;
}

template <size_t P, class T>
T lpf<P, T>::operator()(T&& n) {
	return update(n);
}

template <size_t P, class T>
T lpf<P, T>::update(T&& n) {
	return update(n);
}

