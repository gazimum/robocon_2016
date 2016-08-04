/*
 * wheel_odometry.hpp
 *
 *  Created on: 2016/07/06
 *      Author: tomoya
 */

#ifndef ROBOT_WHEEL_ODOMETRY_HPP_
#define ROBOT_WHEEL_ODOMETRY_HPP_

#include <robot/omni_wheel.hpp>
#include <lpf.hpp>

class wheel_odometry {
public:
	wheel_odometry();
	~wheel_odometry();

	float get_tire_frequency_kHz(int id);
	float get_tire_frequency_Hz(int id);
	float get_tire_advanced_speed_cm_per_sec(int id);
	float get_raw(int id);
	float get_heading_rad();

private:
	static const float _encoder_resolution;
	static const float _tire_radius_cm;
	static const float _tire_encoder_gear_ratio;

	lpf<float> _encoder_raw_data_lpf[omni_wheel::_wheel_num];
};

#endif /* ROBOT_WHEEL_ODOMETRY_HPP_ */
