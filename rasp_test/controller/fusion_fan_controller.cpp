/*
 * fusion_fan_controller.cpp
 *
 *  Created on: 2016/10/04
 *      Author: u
 */

#include <controller/fusion_fan_controller.hpp>
#include <controller/simple_controller.hpp>
#include <i2c/i2c.hpp>

fusion_fan_controller::fusion_fan_controller() {
	// TODO 自動生成されたコンストラクター・スタブ
}

fusion_fan_controller::~fusion_fan_controller() {
	// TODO Auto-generated destructor stub
}

controller_impl* fusion_fan_controller::update() {
	i2c::instance().set("debug_0", 0x02);

	_command["is_enable_arm"] = -1.0f;
	_command["is_enable_moving_object"] = -1.0f;

	if (is_key_rise("controller_switch_1") ||
			is_key_rise("controller_switch_2")) {
		return new simple_controller;
	}
	return this;
}
