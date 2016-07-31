/*
 * controller_impl.hpp
 *
 *  Created on: 2016/07/18
 *      Author: u
 */

#ifndef CONTROLLER_CONTROLLER_IMPL_HPP_
#define CONTROLLER_CONTROLLER_IMPL_HPP_

#include <map>
#include <string>

class controller_impl {
public:
	controller_impl();
	virtual ~controller_impl();

	controller_impl* update(std::map<std::string, int>& controller_state);

	float get(std::string key);

protected:
	virtual controller_impl* update(std::map<std::string, float>& normalized_controller_state) = 0;

	static const float _command_threshold;

	std::map<std::string, float> _command;
};

#endif /* CONTROLLER_CONTROLLER_IMPL_HPP_ */
