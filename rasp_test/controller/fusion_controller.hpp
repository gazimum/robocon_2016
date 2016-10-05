/*
 * fusion_controller.hpp
 *
 *  Created on: 2016/10/04
 *      Author: u
 */

#ifndef CONTROLLER_FUSION_CONTROLLER_HPP_
#define CONTROLLER_FUSION_CONTROLLER_HPP_

#include <controller/arm_controller.hpp>
#include <controller/moving_object_controller.hpp>

class fusion_controller : 	public arm_controller,
							public moving_object_controller {
public:
	fusion_controller();
	virtual ~fusion_controller();
	virtual void reload_config_value() override;

protected:
	virtual controller_impl* update() override;
	virtual void update_pid_index() override;
	virtual void update_lpf_index() override;
};

#endif /* CONTROLLER_FUSION_CONTROLLER_HPP_ */
