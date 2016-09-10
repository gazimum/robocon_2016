/*
 * controller_impl.hpp
 *
 *  Created on: 2016/07/18
 *      Author: u
 */

#ifndef CONTROLLER_CONTROLLER_IMPL_HPP_
#define CONTROLLER_CONTROLLER_IMPL_HPP_

#include <map>
#include <string>
#include <vector>

class controller_impl {
public:
	controller_impl();
	virtual ~controller_impl();

	controller_impl* update(std::map<std::string, int>& controller_state);

	float get(std::string key);
	virtual void reload_ini_file_value() {};

protected:
	virtual controller_impl* update() = 0;
	virtual void update_pid_index() {}

	bool is_key_pushed(std::string name);
	bool is_key_rise(std::string name);
	std::string get_key_by_name(std::string name);

	int read_arm_ability_position_index();
	void update_ini_parser();
	void update_angle();


	void apply_grab();
	void apply_lock();

	static const float _command_threshold;
	static std::map<std::string, size_t> _arm_ability_position_index_dataset;
	static const std::vector<std::string> _arm_ability_name_dataset;
	static bool _is_lock_enable;
	static bool _is_grab_enable;

	std::map<std::string, float> _command;
	std::map<std::string, float> _normalized_controller_state;
	std::map<std::string, float> _prev_normalized_controller_state;
};

#endif /* CONTROLLER_CONTROLLER_IMPL_HPP_ */
