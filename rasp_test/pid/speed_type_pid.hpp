/*
 * speed_type_pid.hpp
 *
 *  Created on: 2016/07/04
 *      Author: tomoya
 */

#ifndef PID_SPEED_TYPE_PID_HPP_
#define PID_SPEED_TYPE_PID_HPP_

#include "pid.hpp"

template <class T>
class speed_type_pid : pid<T> {
public:
	speed_type_pid();
	~speed_type_pid();

	virtual T update(const T& e);

	const T F1;
	const T F2;
	T _J;
	T _mv;
	T _prev_error;
};

#include "speed_type_pid_impl.hpp"

#endif /* PID_SPEED_TYPE_PID_HPP_ */
