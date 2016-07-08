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

	float get_measured_velocity_x();
	float get_measured_velocity_y();
	float get_measured_heading_rad();
};

#endif /* ROBOT_WHEEL_ODOMETRY_HPP_ */
