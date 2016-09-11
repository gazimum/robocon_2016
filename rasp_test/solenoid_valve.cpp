/*
 * solenoid_valve_manager.cpp
 *
 *  Created on: 2016/08/23
 *      Author: u
 */

#include <ini_parser.hpp>
#include <i2c/i2c.hpp>
#include <solenoid_valve.hpp>
#include <controller/controller.hpp>

solenoid_valve::solenoid_valve() : _write_data(0) {
	init();
	controller::instance().add_reload_ini_file_value_function(
			std::bind(&solenoid_valve::init, this)
	);
}

void solenoid_valve::set(std::string name, float p) {
	int i = 0;
	if (p > 0.0f) {
		i = 1;
	};
	_write_data &= ~(static_cast<int>(1) << _name_and_index_dataset[name]);
	_write_data |= i << _name_and_index_dataset[name];

	i2c::instance().set("solenoid_valve", _write_data);
}

void solenoid_valve::init() {
	size_t solenoid_valve_num = ini_parser::instance().get<int>("solenoid_valve", "solenoid_valve_num");
	for (size_t i = 0; i < solenoid_valve_num; ++i) {
		std::string name {
			ini_parser::instance().get<std::string>("solenoid_valve", "solenoid_valve_" + std::to_string(i) + "_name")
		};
		std::string value_key {
			"solenoid_valve_" + std::to_string(i) + "_index"
		};
		_name_and_index_dataset[name] = ini_parser::instance().get<int>("solenoid_valve", value_key);
	}
}
