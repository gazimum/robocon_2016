/*
 * lpf.cpp
 *
 *  Created on: 2016/07/04
 *      Author: tomoya
 */

#include <lpf.hpp>

template <class T>
lpf<T>::lpf(T p, T new_value) : _p(p),
					 	  	  	  _value(new_value) {}

template <class T>
T lpf<T>::operator()(const T& new_value) {
	return update(new_value);
}

template <class T>
T lpf<T>::update(const T& new_value) {
	_value = _value * _p + new_value * (1.0f - _p);
	return _value;
}

template <class T>
T lpf<T>::operator()(T&& new_value) {
	return update(new_value);
}

template <class T>
T lpf<T>::update(T&& new_value) {
	return update(new_value);
}

template <class T>
void lpf<T>::set(float p) {
	_p = p;
}

template <class T>
void lpf<T>::init() {
	_value = T();
}
