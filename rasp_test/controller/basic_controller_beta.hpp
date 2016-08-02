/*
 * basiccontroller.hpp
 *
 *  Created on: 2016/07/18
 *      Author: u
 */
/*
#ifndef CONTROLLER_BASIC_CONTROLLER_HPP_
#define CONTROLLER_BASIC_CONTROLLER_HPP_

#include <controller/controller_impl.hpp>

class basic_controller: public controller_impl {
public:
	basic_controller();
	virtual ~basic_controller();

private:
	virtual controller_impl* update(std::map<std::string, float>& normalized_controller_state);

	void update_arm(std::map<std::string, float>& normalized_controller_state);
	void update_movement(std::map<std::string, float>& normalized_controller_state);
	controller_impl* update_sequence(std::map<std::string, float>& normalized_controller_state);

	bool update_arm_abilities_position_index(std::map<std::string, float>& normalized_controller_state, std::string name);
	void update_arm_abilities_position();

	bool udpate_arm_index_and_adjustment(std::map<std::string, float>& normalized_controller_state);
	void update_arm_adjustment(std::map<std::string, float> normalized_controller_state, std::string name);

	static const std::string _arm_abilities_name[];
	static const std::map<std::string, size_t> _arm_abilities_init_position_index;

	std::map<std::string, float> _arm_adjustment;
	std::map<std::string, size_t> _arm_abilities_position_index;
	std::map<std::string, bool> _prev_ib_buttons_state;
};

#endif*/ /* CONTROLLER_BASIC_CONTROLLER_HPP_ */
