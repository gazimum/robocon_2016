/*
 * moving_object.hpp
 *
 *  Created on: 2016/06/27
 *      Author: tomoya
 */

#ifndef MOVING_OBJECT_HPP_
#define MOVING_OBJECT_HPP_

#include <map>
#include <vector>
#include <pid/position_pid.hpp>
#include <robot/omni_wheel.hpp>

class moving_object {
public:
	moving_object();
	~moving_object();

	void update();

private:
	static const std::vector<std::string> _lpf_name_dataset;
	omni_wheel _omni_wheel;
};

#endif /* MOVING_OBJECT_HPP_ */
