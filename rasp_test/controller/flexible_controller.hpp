/*
 * flexiblecontroller.hpp
 *
 *  Created on: 2016/09/06
 *      Author: u
 */

#ifndef CONTROLLER_FLEXIBLE_CONTROLLER_HPP_
#define CONTROLLER_FLEXIBLE_CONTROLLER_HPP_

#include <controller/controller_impl.hpp>
#include <map>

class flexible_controller : public controller_impl {
public:
	flexible_controller();
	virtual ~flexible_controller();

private:
	virtual controller_impl* update() override;

	controller_impl* update_sequence();
	void update_state_name();
	void update_grab();
	void update_lock();
	void update_movement();
	void update_angle_base();

	int read_arm_abilities_position_index();

	void update_state_by_state_name();

	static std::string _state_name;

	static bool _is_lock_enable;
	static bool _is_grab_enable;

	std::map<std::string, int> _state_index_dataset;
};


#endif /* CONTROLLER_FLEXIBLE_CONTROLLER_HPP_ */
