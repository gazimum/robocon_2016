/*
 * flexiblecontroller.hpp
 *
 *  Created on: 2016/09/06
 *      Author: u
 */

#ifndef CONTROLLER_SIMPLE_CONTROLLER_HPP_
#define CONTROLLER_SIMPLE_CONTROLLER_HPP_

#include <controller/moving_object_controller.hpp>
#include <controller/arm_controller.hpp>
#include <map>

class simple_controller : 	public moving_object_controller,
							public arm_controller {
public:
	simple_controller();
	virtual ~simple_controller();
	virtual void reload_config_value() override;

protected:
	virtual controller_impl* update() override;
	virtual void update_pid_index() override;
	virtual void update_lpf_index() override;

	controller_impl* update_sequence();

};

#endif /* CONTROLLER_simple_controller_HPP_ */
