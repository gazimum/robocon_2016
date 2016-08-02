/*
 * wheel_odometry.hpp
 *
 *  Created on: 2016/07/06
 *      Author: tomoya
 */

#ifndef ROBOT_WHEEL_ODOMETRY_HPP_
#define ROBOT_WHEEL_ODOMETRY_HPP_

class wheel_odometry {
public:
	wheel_odometry();
	~wheel_odometry();

	float get_tire_frequency_kHz(int id);
	float get_tire_advanced_speed_cm_per_sec(int id);
	float get_heading_rad();

private:
	static const int _encoder_resolution;
	static const float _tire_radius_cm;
};

#endif /* ROBOT_WHEEL_ODOMETRY_HPP_ */
