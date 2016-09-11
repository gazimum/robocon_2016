/*
 * potentiometer.cpp
 *
 *  Created on: 2016/09/04
 *      Author: u
 */

#include <potentiometer.hpp>
#include <controller/controller.hpp>
#include <ini_parser.hpp>

const std::map<std::string, serial_connected_mcu::read_id> potentiometer::_read_id_dataset {
	{"arm_length", serial_connected_mcu::POTENTIONMETER1},
	{"arm_height", serial_connected_mcu::POTENTIONMETER3}
};
std::map<std::string, lpf<float>> potentiometer::_lpf{};

potentiometer::potentiometer() {
	for (auto&& i : _read_id_dataset) {
		_lpf.insert(
			std::make_pair(
				i.first,
				lpf<float>()
			)
		);
	}

	init();
	controller::instance().add_reload_ini_file_value_function(
			std::bind(&potentiometer::init, this)
	);
}

float potentiometer::get_position(std::string name) const {
	auto&& i = serial_connected_mcu::serial_connected_mcu_master::instance();

	if (_read_id_dataset.find(name) == _read_id_dataset.end()) {
		throw std::out_of_range(
			"in potentiometer::get_position(std::string name) : \"" + name + "\" is not found in \"_read_id_dataset\""
		);
	}

	float an = i.get(_read_id_dataset.at(name));
	return _lpf.at(name).update(an);
}

void potentiometer::init() {
	for (auto&& i : _lpf) {
		i.second.set(
			ini_parser::instance().get<float>("lpf",  "analog_in_" + i.first + "_lpf_p")
		);
	}
}

