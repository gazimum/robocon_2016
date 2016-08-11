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
#include <cstdio>
#include <robot/wheel_odometry.hpp>
#include <ini_parser.hpp>

#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
/*
 *  1      2
 *
 *      3
 */

const float omni_wheel::_wheel_directions_x[] = {
	cos(-120.0 * M_PI / 180.0),
	cos( 120.0 * M_PI / 180.0),
	cos(   0.0 * M_PI / 180.0)
};

const float omni_wheel::_wheel_directions_y[] = {
	sin(-120.0 * M_PI / 180.0),
	sin( 120.0 * M_PI / 180.0),
	sin(   0.0 * M_PI / 180.0)
};

const float omni_wheel::_wheel_position_angles[] = {
	 30.0f * M_PI / 180.0f,
	150.0f * M_PI / 180.0f,
	-90.0f * M_PI / 180.0f
};

omni_wheel::omni_wheel() : _velocity_x(float()),
						   _velocity_y(float()),
						   _angular_velocity{float()},
						   _tire_frequency_pid{
							   {
								   ini_parser::instance().get<float>("setting", "omni_wheel_tire_frequency_pid_kp"),
								   ini_parser::instance().get<float>("setting", "omni_wheel_tire_frequency_pid_ki"),
								   ini_parser::instance().get<float>("setting", "omni_wheel_tire_frequency_pid_kd")
							   },
							   {
								   ini_parser::instance().get<float>("setting", "omni_wheel_tire_frequency_pid_kp"),
								   ini_parser::instance().get<float>("setting", "omni_wheel_tire_frequency_pid_ki"),
								   ini_parser::instance().get<float>("setting", "omni_wheel_tire_frequency_pid_kd")
							   },
							   {
								   ini_parser::instance().get<float>("setting", "omni_wheel_tire_frequency_pid_kp"),
								   ini_parser::instance().get<float>("setting", "omni_wheel_tire_frequency_pid_ki"),
								   ini_parser::instance().get<float>("setting", "omni_wheel_tire_frequency_pid_kd")
							   }
						   } {
	_wheel_odometry = new wheel_odometry;
}

omni_wheel::~omni_wheel() {}

void omni_wheel::write() {
	/*
	static float mv[_wheel_num] = {};
	static float prev_e[_wheel_num] = {};
	 */
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
	// 回転数制御
	for (size_t i = 0; i < _wheel_num; ++i) {
		float f = _wheel_odometry->get_raw(i) * 1000.0f / ini_parser::instance().get<int>("setting", "encoder_resolution");
		float target_f = p[i] * ini_parser::instance().get<float>("setting", "target_tire_frequency");

		float threshold = 0.01f;
		if (std::fabs(target_f) > threshold) {
			float e = target_f - f;
			p[i] = _tire_frequency_pid[i](e);
			/*
			mv[i] += e * ini_parser::instance().setting<float>("omni_wheel_tire_frequency_pid_kp");
			p[i] = mv[i] + e * ini_parser::instance().setting<float>("omni_wheel_tire_frequency_pid_kd");
			prev_e[i] = e;
			*/
		} else {
			_tire_frequency_pid[i].init();
		}

		std::cout << target_f << " " << f << ":";

		p[i] = std::max(-1.0f, std::min(p[i], 1.0f));
	}
	std::cout << std::endl;


	for (size_t i = 0; i < _wheel_num; ++i) {
		i2c::instance().set("wheel" + std::to_string(i), p[i]);
	}
}

void omni_wheel::set_velocity(float x, float y) {
	float l = x * x + y * y;
	if (l > 1.0f) { // ベクトルの大きさが1より大きいなら1に調整する
		l = sqrt(l);
		x /= l;
		y /= l;
	}
	_velocity_x = x;
	_velocity_y = y;
}

void omni_wheel::set_angular_velocity(float v) {
	_angular_velocity = std::max(-1.0f, std::min(1.0f, v));
}

void omni_wheel::set_tire_frequency_pid_coeff(float kp, float ki, float kd) {
	for (size_t i = 0; i < _wheel_num; ++i) {
		_tire_frequency_pid[i].update_coeff(kp, ki, kd);
	}
}

