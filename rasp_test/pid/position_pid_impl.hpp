/*
 * position_pid_impl.hpp
 *
 *  Created on: 2016/08/08
 *      Author: tomoya
 */

#ifndef PID_POSITION_PID_IMPL_HPP_
#define PID_POSITION_PID_IMPL_HPP_

template <class T>
position_pid<T>::position_pid(const T& kp, const T& ki, const T& kd) :	pid<T>(kp, ki, kd) {}

template <class T>
position_pid<T>::~position_pid() {}

template <class T>
T position_pid<T>::update(const T& e) {
	static float prev_e = 0.0f;
	static float accum_e = 0.0f;
	static float mv = 0.0f;

	// todo : implement ini file
	const float threshold = 0.01f;
	if (std::abs(e) < threshold) {
		prev_e = 0.0f;
		accum_e = 0.0f;
		mv = 0.0f;
		return mv;
	}

	float dmv = pid<T>::_kp * e;
	dmv += pid<T>::_ki * (e - prev_e);
	dmv += pid<T>::_kd * accum_e;

	prev_e = e;

	mv += dmv;
	return mv;
}

#endif /* PID_POSITION_PID_IMPL_HPP_ */
