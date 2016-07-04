/*
 * lpf.h
 *
 *  Created on: 2016/07/04
 *      Author: tomoya
 */

#ifndef LPF_HPP_
#define LPF_HPP_

template <size_t P, class T>
class lpf {
public:
	explicit lpf(T n = T());

	inline T operator()(const T& n);
	inline T update(const T& n);
	inline T operator()(T&& n);
	inline T update(T&& n);

private:
	T _value;
};

#include "lpf_impl.hpp"

typedef lpf<90, float> lpf_90f;
typedef lpf<90, float> basic_low_pass_filter;

#endif /* LPF_HPP_ */
