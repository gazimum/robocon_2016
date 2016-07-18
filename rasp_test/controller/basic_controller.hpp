/*
 * basiccontroller.hpp
 *
 *  Created on: 2016/07/18
 *      Author: u
 */

#ifndef CONTROLLER_BASIC_CONTROLLER_HPP_
#define CONTROLLER_BASIC_CONTROLLER_HPP_

#include <controller/controller_impl.hpp>

class basic_controller: public controller_impl {
public:
	basic_controller();
	virtual ~basic_controller();

private:
	virtual controller_impl* update(std::map<std::string, float>& command);

	void update_arm(std::map<std::string, float>& command);
	void update_movement(std::map<std::string, float>& command);
	controller_impl* update_sequence(std::map<std::string, float>& command);

	bool update_arm_abilities_position_index(std::map<std::string, float>& normalized_controller_state);
	void update_arm_abilities_position();

	static const std::string _arm_abilities_name[];
	static const std::map<std::string, size_t> _arm_abilities_init_position_index;

	std::map<std::string, float> _arm_adjusting_values;
	std::map<std::string, size_t> _arm_abilities_position_index;
};

#endif /* CONTROLLER_BASIC_CONTROLLER_HPP_ */
