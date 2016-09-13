/*
 * simplecontroller.hpp
 *
 *  Created on: 2016/08/17
 *      Author: u
 */

#ifndef CONTROLLER_SIMPLE_CONTROLLER_HPP_
#define CONTROLLER_SIMPLE_CONTROLLER_HPP_

#include <controller/moving_object_controller.hpp>
#include <state_machine/state_machine.hpp>
#include <chrono>
#include <map>

class simple_controller : public moving_object_controller {
public:
	simple_controller();
	virtual ~simple_controller();
	virtual void reload_config_value() override;

private:
	virtual controller_impl* update() override;
	virtual void update_pid_index() override;
	virtual void update_lpf_index() override;

	controller_impl* update_sequence();
	void update_lock();

	int read_arm_abilities_position_index();

	std::string release();
	std::string height_low();
	std::string grab();
	std::string height_adjust();

	void update_state_name();
	void update_state_by_state_name();

	static std::string _state_name;
	static state_machine _state_machine;

	std::chrono::system_clock::time_point _time;
	std::map<std::string, int> _state_index_dataset;
};

#endif /* CONTROLLER_SIMPLE_CONTROLLER_HPP_ */
