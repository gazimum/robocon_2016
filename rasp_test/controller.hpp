/*
 * controller.hpp
 *
 *  Created on: 2016/07/05
 *      Author: tomoya
 */

#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <string>
#include "singleton.hpp"

class controller : singleton<controller> {
public:
	void operator()();
	void get();

	inline float get_velocity_x() const;
	inline float get_velocity_y() const;
	inline float get_angular_velocity() const;

private:
	controller() {}

	static constexpr float _controller_analog_coeff = 1.0f / 32767.0f;
	static const std::string _controller_name;

	float _velocity_x;
	float _velocity_y;
	float _angular_velocity;
};

#endif /* CONTROLLER_HPP_ */
