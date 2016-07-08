/*
 * omniwheel.cpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#include "omni_wheel.hpp"
#include <cmath>
#include <algorithm>
#include "../i2c/i2c.hpp"
#include "../i2c/i2c_profile.hpp"

/*
 *  1      2
 *
 *      3
 */
const float omni_wheel::_wheel_directions_x[] = {
		1.0f / sqrt(2.0f),
		1.0f / sqrt(2.0f),
		-1.0f
};

const float omni_wheel::_wheel_directions_y[] = {
		 1.0f / sqrt(2.0f),
		-1.0f / sqrt(2.0f),
		 0.0f
};

omni_wheel::omni_wheel() : _velocity_x(float()),
						   _velocity_y(float()),
						   _velocity_propotion{0.5f},
						   _angular_velocity{float()} {}

omni_wheel::~omni_wheel() {}

void omni_wheel::write() {
	float p[_wheel_num];

	for (size_t i = 0; i < _wheel_num; ++i) {
		p[i] = _velocity_x * _wheel_directions_x[i] + _velocity_y * _wheel_directions_y[i];
		p[i] *= _velocity_propotion;
		p[i] += (1.0f - _velocity_propotion) * _angular_velocity;
	}
	for (size_t i = 0; i < _wheel_num; ++i) {
		i2c::instance().set(i2c_device_name[i], p[i]);
	}
}

void omni_wheel::set_velocity(float x, float y) {
	float l2 = x * x + y * y;
	if (l2 > 1.0f) { // ベクトルの大きさが1より大きいなら1に調整する
		float l = sqrt(l2);
		x /= l;
		y /= l;
	}
	_velocity_x = x;
	_velocity_y = y;
}

void omni_wheel::set_angular_velocity(float v) {
	_angular_velocity = std::max(-1.0f, std::min(1.0f, v));
}
