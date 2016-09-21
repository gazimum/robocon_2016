/*
 * moving_object.hpp
 *
 *  Created on: 2016/06/27
 *      Author: tomoya
 */

#ifndef MOVING_OBJECT_HPP_
#define MOVING_OBJECT_HPP_

#include <vector>

class moving_object {
public:
	moving_object();
	virtual ~moving_object();

	void update();
	void set_velocity(float vx, float vy);
	void set_angular_velocity(float av);
	void set_target_heading_rad(float heading_rad);

protected:
	virtual void write() = 0;

	float _velocity_x;
	float _velocity_y;
	float _angular_velocity;
	float _target_heading_rad;

private:
	static const std::vector<std::string> _lpf_name_dataset;
};

#endif /* MOVING_OBJECT_HPP_ */
