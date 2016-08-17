/*
 * speed_pid.hpp
 *
 *  Created on: 2016/07/04
 *      Author: tomoya
 */

#ifndef PID_SPEED_PID_HPP_
#define PID_SPEED_PID_HPP_

#include "pid.hpp"

template <class T>
class speed_pid : public pid<T> {
public:
	speed_pid(const T& kp = T(), const T& ki = T(), const T& kd = T());
	virtual ~speed_pid();

	virtual T update(const T& e) override;
	virtual void init() override;

private:
	virtual void update_specific_coeff() override;

	T _F1;
	T _F2;
	T _J;
	T _prev_error;
};

#include <pid/speed_pid_impl.hpp>

#endif /* PID_SPEED_PID_HPP_ */
