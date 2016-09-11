/*
 * flexiblecontroller.hpp
 *
 *  Created on: 2016/09/06
 *      Author: u
 */

#ifndef CONTROLLER_FLEXIBLE_CONTROLLER_HPP_
#define CONTROLLER_FLEXIBLE_CONTROLLER_HPP_

#include <controller/moving_object_controller.hpp>
#include <map>

class flexible_controller : public moving_object_controller {
public:
	flexible_controller();
	virtual ~flexible_controller();
	virtual void reload_ini_file_value() override;

private:
	virtual controller_impl* update() override;
	virtual void update_pid_index() override;
	virtual void update_lpf_index() override;

	controller_impl* update_sequence();
	void update_state_name();
	void update_grab();
	void update_lock();
	void update_angle_base();

	int read_arm_abilities_position_index();

	void update_state_by_state_name();

	static std::string _state_name;

	std::map<std::string, int> _state_index_dataset;
};


#endif /* CONTROLLER_FLEXIBLE_CONTROLLER_HPP_ */
