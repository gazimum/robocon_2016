/*
 * pid.hpp
 *
 *  Created on: 2016/07/04
 *      Author: tomoya
 */

#ifndef PID_HPP_
#define PID_HPP_

template <class T>
class pid {
public:
	pid(const T& kp, const T& ki, const T& kd);
	virtual ~pid() {};

	T operator()(const T& e);
	T operator()(T&& e);
	virtual T update(const T& e) = 0;
	T update(T&& e);

protected:
	const T _kp;
	const T _ki;
	const T _kd;
};

#include "pid_impl.hpp"

#endif /* PID_HPP_ */
