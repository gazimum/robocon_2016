/*
 * fusion_controller.cpp
 *
 *  Created on: 2016/10/04
 *      Author: u
 */

#include <controller/fusion_controller.hpp>
#include <controller/simple_controller.hpp>
#include <i2c/i2c.hpp>
#include <pid/pid_manager.hpp>
#include <lpf/lpf_manager.hpp>
#include <config.hpp>

fusion_controller::fusion_controller() {
	reload_config_value();
}

fusion_controller::~fusion_controller() {}

controller_impl* fusion_controller::update() {
	i2c::instance().set("debug_0", 0x04);

	update_arm();

	_command["is_enable_moving_object"] = -1.0f;

	if (is_key_rise("controller_switch_1") ||
			is_key_rise("controller_switch_2")) {
		return new simple_controller;
	}
	return this;
}

void fusion_controller::update_pid_index() {
	pid_manager<float>::instance().set_index(_state_index_dataset.at(_state_name));
	pid_manager<float>::instance().config();
}

void fusion_controller::update_lpf_index() {
	lpf_manager<float>::instance().set_index(_state_index_dataset.at(_state_name));
	lpf_manager<float>::instance().config();
}


void fusion_controller::reload_config_value() {
	size_t arm_state_num = config::instance().get<int>("arm_state", "arm_state_num");
	for (size_t i = 0; i < arm_state_num; ++i) {
		std::string name = config::instance().get<std::string>("arm_state", "state_" + std::to_string(i) + "_name");
		_state_index_dataset[name] = i;
	}
}

