/*
 * basiccontroller.hpp
 *
 *  Created on: 2016/07/18
 *      Author: u
 */

#ifndef CONTROLLER_BASIC_CONTROLLER_HPP_
#define CONTROLLER_BASIC_CONTROLLER_HPP_

#include <controller/moving_object_controller.hpp>

class basic_controller : public moving_object_controller {
public:
	basic_controller();
	virtual ~basic_controller();

private:
	virtual controller_impl* update() override;

	void update_arm();
	controller_impl* update_sequence();

	bool udpate_arm_index_and_adjustment();
	void update_arm_ability_position();

	bool update_arm_ability_position_index(std::string name);
	void update_arm_adjustment(std::string name);

	void teaching(std::string name);
	void write_ini_file();

	std::map<std::string, float> _arm_adjustment;
	std::map<std::string, bool> _prev_ib_buttons_state;
};

#endif /* CONTROLLER_BASIC_CONTROLLER_HPP_ */
