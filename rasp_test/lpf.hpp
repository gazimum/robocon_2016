/*
 * lpf.h
 *
 *  Created on: 2016/07/04
 *      Author: tomoya
 */

#ifndef LPF_HPP_
#define LPF_HPP_

template <class T>
class lpf {
public:
	lpf(T p = T(), T n = T());

	inline T operator()(const T& new_value);
	inline T update(const T& new_value);
	inline T operator()(T&& new_value);
	inline T update(T&& new_value);

	void set(float p);
	void init();

private:
	float _p;
	T _value;
};

#include "lpf_impl.hpp"

typedef lpf<float> lpf_f;
typedef lpf<float> basic_low_pass_filter;

#endif /* LPF_HPP_ */
