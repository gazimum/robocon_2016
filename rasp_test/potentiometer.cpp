/*
 * potentiometer.cpp
 *
 *  Created on: 2016/09/04
 *      Author: u
 */

#include <potentiometer.hpp>

const std::map<std::string, serial_connected_mcu::read_id> potentiometer::_read_id_dataset {
	{"length", serial_connected_mcu::POTENTIONMETER1},
	{"height", serial_connected_mcu::POTENTIONMETER3}
};

potentiometer::potentiometer() {}

float potentiometer::get_position(std::string name) {
	auto&& i = serial_connected_mcu::serial_connected_mcu_master::instance();
	return i.get(_read_id_dataset.at(name));
}

