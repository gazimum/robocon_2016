/*
 * flexiblecontroller.cpp
 *
 *  Created on: 2016/09/06
 *      Author: u
 */

#include <iostream>
#include <string>
#include <controller/controller.hpp>
#include <controller/simple_controller.hpp>
#include <controller/basic_controller.hpp>
#include <controller/flexible_controller.hpp>
#include <controller/fusion_fan_controller.hpp>
#include <controller/fusion_controller.hpp>
#include <pid/pid_manager.hpp>
#include <config.hpp>
#include <lpf/lpf_manager.hpp>
#include <i2c/i2c.hpp>

simple_controller::simple_controller() {
	reload_config_value();
}

simple_controller::~simple_controller() {}

controller_impl* simple_controller::update() {
	i2c::instance().set("debug_0", 0x01);

	update_movement();
	update_arm();
	return update_sequence();
}

void simple_controller::update_pid_index() {
	pid_manager<float>::instance().set_index(_state_index_dataset.at(_state_name));
	pid_manager<float>::instance().config();
}

void simple_controller::update_lpf_index() {
	lpf_manager<float>::instance().set_index(_state_index_dataset.at(_state_name));
	lpf_manager<float>::instance().config();
}

controller_impl* simple_controller::update_sequence() {
	if (controller_impl::is_key_rise("controller_switch_1")) {
		return new fusion_controller;
	}
	if (controller_impl::is_key_rise("controller_switch_2")) {
		return new fusion_fan_controller;
	}
	return this;
}

void simple_controller::reload_config_value() {
	size_t arm_state_num = config::instance().get<int>("arm_state", "arm_state_num");
	for (size_t i = 0; i < arm_state_num; ++i) {
		std::string name = config::instance().get<std::string>("arm_state", "state_" + std::to_string(i) + "_name");
		_state_index_dataset[name] = i;
	}
}

