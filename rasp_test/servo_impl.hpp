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
#include <i2c/i2c.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>

template <class T>
servo<T>::servo(int id, float kp, float ki, float kd) : _id(id),
														_position(float()),
														_pid(kp, ki, kd) {}

template <class T>
float servo<T>::operator()(float p) {
	set(p);
}

template <class T>
float servo<T>::set(float p) {
	p = std::max(-1.0f, std::min(p, 1.0f));
	float current_pos = serial_connected_mcu::serial_connected_mcu_master::instance().get(_id);
	current_pos /= 32768.0f;
	_position = _pid(p - current_pos);
	i2c::instance().set("servo" + std::to_string(_id), _position);
	return _position;
}

template <class T>
float servo<T>::get() const {
	return _position;
}

#endif
