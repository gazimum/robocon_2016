/*
 * omniwheel.cpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#include "omni_wheel.hpp"
#include <cmath>
#include <algorithm>
#include <i2c/i2c.hpp>
#include <iostream>
/*
 *  1      2
 *
 *      3
 */
const float omni_wheel::_wheel_directions_x[] = {
	cos( 60.0  * M_PI / 180.0),
	cos(-60.0  * M_PI / 180.0),
	cos( 180.0 * M_PI / 180.0)
};

const float omni_wheel::_wheel_directions_y[] = {
	sin( 60.0 * M_PI / 180.0),
	sin(-60.0 * M_PI / 180.0),
	sin(180.0 * M_PI / 180.0)
};

const float omni_wheel::_wheel_position_angles[] = {
	 30.0f * M_PI / 180.0f,
	150.0f * M_PI / 180.0f,
	-90.0f * M_PI / 180.0f
};

omni_wheel::omni_wheel() : _velocity_x(float()),
						   _velocity_y(float()),
						   _angular_velocity{float()} {}

omni_wheel::~omni_wheel() {}

void omni_wheel::write() {
	float p[_wheel_num];

	float max = 0.0f;
	for (size_t i = 0; i < _wheel_num; ++i) {
		p[i] = _velocity_x * _wheel_directions_x[i] + _velocity_y * _wheel_directions_y[i];
		p[i] += _angular_velocity;

		if (std::abs(p[i]) > 1.0f ) {
			max = std::max(std::abs(p[i]), max);
		}
	}

	if (max != 0.0f) {
		for (size_t i = 0; i < _wheel_num; ++i) {
			p[i] /= max;
		}
	}

	std::cout << max << " ";
	for (size_t i = 0; i < _wheel_num; ++i) {
		std::cout << p[i] << " ";
		i2c::instance().set("wheel" + std::to_string(i), p[i]);
	}
	std::cout << std::endl;
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
