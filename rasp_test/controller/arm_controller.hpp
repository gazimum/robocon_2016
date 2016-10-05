/*
 * arm_controller.hpp
 *
 *  Created on: 2016/10/05
 *      Author: u
 */

#ifndef CONTROLLER_ARM_CONTROLLER_HPP_
#define CONTROLLER_ARM_CONTROLLER_HPP_

#include <controller/controller_impl.hpp>

class arm_controller : public virtual controller_impl {
public:
	arm_controller();
	virtual ~arm_controller();

protected:
	void update_arm();

	void update_state_name();
	void adjustment();
	void update_grab();
	void update_lock();
	void update_angle_base();

	int read_arm_abilities_position_index();
	void update_state_by_state_name();

	static std::string _state_name;

	std::map<std::string, float> _adjustment_dataset;
	std::map<std::string, int> _state_index_dataset;
};

#endif /* CONTROLLER_ARM_CONTROLLER_HPP_ */
