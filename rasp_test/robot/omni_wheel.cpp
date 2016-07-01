/*
 * omniwheel.cpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#include "omni_wheel.hpp"
#include <boost/numeric/ublas/matrix.hpp>
#include <cmath>
#include <algorithm>
#include "../i2c/i2c.hpp"
#include "../i2c/i2c_profile.hpp"

#include <iostream>

/*
 *  1      2
 *
 *      3
 */
const omni_wheel::vector omni_wheel::_wheel_directions[_wheel_num] = {
		vector{ 1.0f / sqrt(2.0f),  1.0f / sqrt(2.0f)},
		vector{ 1.0f / sqrt(2.0f), -1.0f / sqrt(2.0f)},
		vector{-1.0f, 0.0f}
};

omni_wheel::omni_wheel() : _velocity_propotion{0.5f},
						   _angular_velocity{float()} {}

omni_wheel::~omni_wheel() {}

void omni_wheel::write() {
	float p[_wheel_num];
	for (size_t i = 0; i < _wheel_num; ++i) {
		p[i] = boost::numeric::ublas::inner_prod(_velocity, _wheel_directions[i]);
		p[i] *= _velocity_propotion;
		p[i] += (1.0f - _velocity_propotion) * _angular_velocity;
	}
	for (size_t i = 0; i < _wheel_num; ++i) {
		i2c::instance().set(i2c_device_name[i], p[i]);
	}
}

void omni_wheel::set_velocity(float x, float y) {
	std::cout << x << ", " << y << std::endl;

	_velocity = vector{x, y};

	// ベクトルの大きさが1より大きいなら1に調整する
	float l = abs(boost::numeric::ublas::norm_2(_velocity));
	if (l > 1.0f) {
		_velocity /= l;
	}
}

void omni_wheel::set_angular_velocity(float v) {
	_angular_velocity = std::max(0.0f, std::min(1.0f, v));
}
