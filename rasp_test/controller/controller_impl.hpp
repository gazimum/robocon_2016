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
#include <server_shared_data.hpp>

class controller_impl {
public:
	using container_type = std::map<std::string, float>;

	static const float _command_threshold;

	controller_impl();
	virtual ~controller_impl();

	controller_impl* update(const server_shared_data::container_type& controller_state);

	container_type get_command();
	virtual void reload_config_value() {};

protected:
	virtual controller_impl* update() = 0;
	virtual void update_pid_index() {}
	virtual void update_lpf_index() {}

	bool is_key_pushed(std::string name, float threshold = _command_threshold);
	bool is_key_rise(std::string name, float threshold = _command_threshold);
	std::string get_key_by_name(std::string name);

	int read_arm_ability_position_index();
	void update_config();
	void update_angle();

	void apply_grab();
	void apply_lock();

	static std::map<std::string, size_t> _arm_ability_position_index_dataset;
	static const std::vector<std::string> _arm_ability_name_dataset;
	static bool _is_lock_enable;
	static bool _is_grab_enable;

	container_type _command;
	container_type _normalized_controller_state;
	container_type _prev_normalized_controller_state;

	server_shared_data::container_type _server_shared_data;

	std::string _mode_name;
	std::string _my_controller_name;
};

#endif /* CONTROLLER_CONTROLLER_IMPL_HPP_ */
