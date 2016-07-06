/*
 * controller.hpp
 *
 *  Created on: 2016/07/05
 *      Author: tomoya
 */

#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <string>
#include <map>
#include "singleton.hpp"

class controller : public singleton<controller> {
public:
	void update();
	float operator()(std::string key);
	float get(std::string key);

private:
	friend class singleton<controller>;
	controller();

	static constexpr float _controller_analog_coeff = 1.0f / 32767.0f;
	static const std::string _my_controller_name;

	std::map<std::string, float> _controller_map;
};

#endif /* CONTROLLER_HPP_ */
