/*
 * basiccontroller.hpp
 *
 *  Created on: 2016/07/18
 *      Author: u
 */

#ifndef CONTROLLER_BASIC_CONTROLLER_HPP_
#define CONTROLLER_BASIC_CONTROLLER_HPP_

#include <controller/controller_impl.hpp>
#include <state_machine/state_machine.hpp>

class basic_controller : public controller_impl {
public:
	basic_controller();
	virtual ~basic_controller();

private:
	virtual controller_impl* update() override;

	void update_ini_parser();

	void update_arm();
	void update_movement();
	controller_impl* update_sequence();

	bool update_arm_abilities_position_index(std::string name);
	void update_arm_abilities_position();

	bool udpate_arm_index_and_adjustment();
	void update_arm_adjustment(std::string name);

	static const std::string _arm_abilities_name[];
	static const std::map<std::string, size_t> _arm_abilities_init_position_index;

	std::map<std::string, float> _arm_adjustment;
	std::map<std::string, size_t> _arm_abilities_position_index;
	std::map<std::string, bool> _prev_ib_buttons_state;
};

#endif /* CONTROLLER_BASIC_CONTROLLER_HPP_ */
