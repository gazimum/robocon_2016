/*
 * wheel_odometry.hpp
 *
 *  Created on: 2016/07/06
 *      Author: tomoya
 */

#ifndef ROBOT_WHEEL_ODOMETRY_HPP_
#define ROBOT_WHEEL_ODOMETRY_HPP_

#include <map>
#include <memory>
#include <robot/omni_wheel.hpp>
#include <singleton.hpp>

class wheel_odometry : public singleton<wheel_odometry> {
public:
	float get_tire_frequency_kHz(int index);
	float get_tire_frequency_Hz(int index);
	float get_tire_advanced_speed_cm_per_sec(int index);
	float get_raw(int index);
	float get_heading_rad();

private:
	friend class singleton<wheel_odometry>;
	wheel_odometry();

	std::unique_ptr<float[]> _prev_raw;
	std::unique_ptr<float[]> _raw_offset;
};

#endif /* ROBOT_WHEEL_ODOMETRY_HPP_ */
