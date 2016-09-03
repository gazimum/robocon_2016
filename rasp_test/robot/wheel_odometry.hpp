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
	static float get_tire_frequency_kHz(int id);
	static float get_tire_frequency_Hz(int id);
	static float get_tire_advanced_speed_cm_per_sec(int index);
	static float get_raw(int id);
	static float get_heading_rad();
private:
	wheel_odometry();
};

#endif /* ROBOT_WHEEL_ODOMETRY_HPP_ */
