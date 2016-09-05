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
#include <vector>

class controller_impl {
public:
	controller_impl();
	virtual ~controller_impl();

	controller_impl* update(std::map<std::string, int>& controller_state);

	float get(std::string key);

protected:
	virtual controller_impl* update() = 0;

	bool is_key_pushed(std::string key);
	bool is_key_rise(std::string key);

	int read_arm_ability_position_index();

	static const float _command_threshold;
	static std::map<std::string, size_t> _arm_ability_position_index_dataset;
	static const std::vector<std::string> _arm_ability_name_dataset;

	std::map<std::string, float> _command;
	std::map<std::string, float> _normalized_controller_state;
	std::map<std::string, float> _prev_normalized_controller_state;
};

#endif /* CONTROLLER_CONTROLLER_IMPL_HPP_ */
