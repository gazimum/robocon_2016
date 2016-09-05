/*
 * simplecontroller.hpp
 *
 *  Created on: 2016/08/17
 *      Author: u
 */

#ifndef CONTROLLER_SIMPLE_CONTROLLER_HPP_
#define CONTROLLER_SIMPLE_CONTROLLER_HPP_

#include <controller/controller_impl.hpp>
#include <state_machine/state_machine.hpp>
#include <chrono>
#include <map>

class simple_controller : public controller_impl {
public:
	simple_controller();
	virtual ~simple_controller();

private:
	virtual controller_impl* update() override;

	void update_ini_parser();

	controller_impl* update_sequence();
	void update_lock();
	void update_movement();
	void update_angle_base();

	int read_arm_abilities_position_index();

	std::string release();
	std::string height_low();
	std::string grab();
	std::string height_adjust();

	void update_state_name();
	void update_state_by_state_name();

	std::chrono::system_clock::time_point _time;

	bool _is_lock_enable;

	std::string _state_name;
	state_machine _state_machine;
	std::map<std::string, int> _state_index_dataset;
};

#endif /* CONTROLLER_SIMPLE_CONTROLLER_HPP_ */
