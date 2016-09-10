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
	virtual T update(const T& e) = 0;

	virtual void init() = 0;
	void update_coeff(float kp, float ki, float kd);

protected:
	virtual void update_specific_coeff() {};

	T _kp;
	T _ki;
	T _kd;
	T _mv;
};

#include "pid_impl.hpp"

#endif /* PID_HPP_ */
