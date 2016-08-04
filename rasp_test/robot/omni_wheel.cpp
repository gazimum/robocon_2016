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

/*
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
									   ini_parser::instance().setting<float>("omni_wheel_tire_frequency_pid_kp"),
									   ini_parser::instance().setting<float>("omni_wheel_tire_frequency_pid_ki"),
									   ini_parser::instance().setting<float>("omni_wheel_tire_frequency_pid_kd")
								   },
								   {
									   ini_parser::instance().setting<float>("omni_wheel_tire_frequency_pid_kp"),
									   ini_parser::instance().setting<float>("omni_wheel_tire_frequency_pid_ki"),
									   ini_parser::instance().setting<float>("omni_wheel_tire_frequency_pid_kd")
								   },
								   {
									   ini_parser::instance().setting<float>("omni_wheel_tire_frequency_pid_kp"),
									   ini_parser::instance().setting<float>("omni_wheel_tire_frequency_pid_ki"),
									   ini_parser::instance().setting<float>("omni_wheel_tire_frequency_pid_kd")
								   }
							   } {
	_wheel_odometry = new wheel_odometry;
}

omni_wheel::~omni_wheel() {}

float sign(float v) {
	if (v < 0.0f) {
		return -1.0f;
	}
	return 1.0f;
}

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

	float max_v = 200.0f;
	// 回転数制御
	for (size_t i = 0; i < _wheel_num; ++i) {
		//p[i] = p[i] * 300.0f - _wheel_odometry.get_tire_advanced_speed_cm_per_sec(i);

		/*
		float v = _wheel_odometry->get_tire_advanced_speed_cm_per_sec(i);
		p[i] = _tire_frequency_pid[i](p[i] * max_v - v);
		std::cout << v << " " << p[i] << ":";
		*/

		float f = _wheel_odometry->get_raw(i) / 1200.0f;
		p[i] = _tire_frequency_pid[i]((p[i] * 10.0f - f) * 20.0f);
		std::cout << f << " " << p[i] << ":";

		//if (p[i] != 0.0f) {
			//p[i] = sign(p[i]) - _wheel_odometry.get_tire_advanced_speed_cm_per_sec(i) / std::abs(p[i] * 300.0f);
			//p[i] = (p[i] * 300.0f - _wheel_odometry.get_tire_advanced_speed_cm_per_sec(i)) / std::abs(p[i] * 300.0f);
			//p[i] = (p[i] * 300.0f - v) / std::abs(p[i] * 300.0f);

			//std::cout << v << " " << p[i] << " ";

			//p[i] += 0.1f * (p[i] * 250.0f - v) / (std::abs(p[i]) * 250.0f);
			//p[i] = (v - p[i] * 250.0f) / (std::abs(p[i]) * 250.0f);
			//p[i] = (p[i] * max_v - v) / (max_v + std::abs(v));
			//p[i] -= p[i] - v / max_v;

			//std::cout << p[i] << std::endl;

/*
			std::cout << v << " ";
			std::cout << p[i] << ", ";
*/
			//p[i] = (p[i] * 250.0f - v) * 0.0005f;
			//std::cout << serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::ENCODER1 + i) << " ";
		//}

		//p[i] *= 0.05f;
		//p[i] *= 2.5f;
		p[i] = std::max(-1.0f, std::min(p[i], 1.0f));

		//std::cout << serial_connected_mcu::serial_connected_mcu_master::instance().get(serial_connected_mcu::ENCODER1 + i) << " ";
	}
	std::cout << std::endl;


	for (size_t i = 0; i < _wheel_num; ++i) {
		i2c::instance().set("wheel" + std::to_string(i), p[i]);
	}

	//static float lpf[3] = {};
	/*
	printf("\r");
	for (size_t i = 0; i < 3; ++i) {
		//lpf[i] = lpf[i] * 0.9f + _wheel_odometry.get_tire_advanced_speed_cm_per_sec(i) * 0.1f;
		//std::cout << lpf[i] << " ";
		std::cout << _wheel_odometry.get_tire_advanced_speed_cm_per_sec(i) << " ";
	}
	*/
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
