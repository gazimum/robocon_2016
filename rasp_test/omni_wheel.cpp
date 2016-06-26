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
//#include <math.h>

/*
 *  1      2
 *
 *      3
 */
const omni_wheel::unit_vector omni_wheel::_wheel_directions[_wheel_num] = {
		unit_vector( 1.0f / sqrt(2.0f),  1.0f / sqrt(2.0f)),
		unit_vector( 1.0f / sqrt(2.0f), -1.0f / sqrt(2.0f)),
		unit_vector(-1.0f, 0.0f)
};

omni_wheel::omni_wheel() : _velocity_propotion(0.5f),
						   _angular_velocity(float()) {}

omni_wheel::~omni_wheel() {}

void omni_wheel::update() {
	float p[_wheel_num];
	for (size_t i = 0; i < _wheel_num; ++i) {
		p[i] = boost::numeric::ublas::inner_prod(_velocity, _wheel_directions[i]);
		p[i] *= _velocity_propotion;
		p[i] += (1.0f - _velocity_propotion) * _angular_velocity;
	}
}

void omni_wheel::set_velocity(vector v) {
	// ベクトルの大きさが1より大きいなら1に調整する
	if (boost::numeric::ublas::norm_2(v) > 1.0f) {
		v /= boost::numeric::ublas::norm_2(v);
	}
	_velocity = v;
}

void omni_wheel::set_angular_velocity(float v) {
	_angular_velocity = std::max(0.0f, std::min(1.0f, v));
}
