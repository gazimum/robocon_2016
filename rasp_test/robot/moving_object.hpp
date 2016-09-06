/*
 * moving_object.hpp
 *
 *  Created on: 2016/06/27
 *      Author: tomoya
 */

#ifndef MOVING_OBJECT_HPP_
#define MOVING_OBJECT_HPP_

#include <map>
#include <pid/position_pid.hpp>
#include <robot/omni_wheel.hpp>
#include <lpf.hpp>

class moving_object {
public:
	moving_object();
	~moving_object();

	void update();

private:
	void enable_lpf();
	void disable_lpf();
	void init_lpf();

	omni_wheel _omni_wheel;
	std::map<std::string, lpf<float>> _command_lpf;
};

#endif /* MOVING_OBJECT_HPP_ */
