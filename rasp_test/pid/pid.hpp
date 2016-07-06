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
	pid(const T& kp, const T& ki, const T& kd, const T& t);
	virtual ~pid() {};

	void operator()(const T& e);
	void operator()(T&& e);
	virtual void update(const T& e) = 0;
	void update(T&& e);

private:
	const T _kp;
	const T _ki;
	const T _kd;
	const T _T;	// サンプリング周期
};

#include "pid_impl.hpp"

#endif /* PID_HPP_ */
