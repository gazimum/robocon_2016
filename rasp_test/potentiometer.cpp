/*
 * potentiometer.cpp
 *
 *  Created on: 2016/09/04
 *      Author: u
 */

#include <potentiometer.hpp>
#include <controller/controller.hpp>
#include <ini_parser.hpp>
#include <lpf/lpf_manager.hpp>

const std::map<std::string, serial_connected_mcu::read_id> potentiometer::_read_id_dataset {
	{"arm_length", serial_connected_mcu::POTENTIONMETER1},
	{"arm_height", serial_connected_mcu::POTENTIONMETER3}
};

potentiometer::potentiometer() {
	for (auto&& i : _read_id_dataset) {
		lpf_manager<float>::instance().add<lpf<float>>(i.first);
		_filtered_value_dataset[i] = 0.0f;
	}
}

float potentiometer::get_position(std::string name) const {
	if (_filtered_value_dataset.find(name) == _filtered_value_dataset.end()) {
		throw std::out_of_range(
			"in potentiometer::get_position(std::string name) : \"" + name + "\" is not found in \"_filtered_value_dataset\""
		);
	}
	return _filtered_value_dataset.at(name);
}

void potentiometer::update() {
	for (const auto& i : _read_id_dataset) {
		auto&& serial = serial_connected_mcu::serial_connected_mcu_master::instance();
		float an = serial.get(i.second);
		_filtered_value_dataset[i.first] = lpf_manager<float>::instance().get(i.first).update(an);
	}
}
