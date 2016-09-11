/*
 * config_pid.cpp
 *
 *  Created on: 2016/09/10
 *      Author: u
 */

#include <ini_parser.hpp>
#include <pid/pid_manager.hpp>
#include <memory>

template <class T>
pid_manager<T>::pid_manager() : _pid_index(int()) {}

template <class T>
void pid_manager<T>::set_index(int index) {
	_pid_index = index;
}

template <class T>
void pid_manager<T>::config() {
	for (auto&& i : _pid_dataset) {
		std::string index_key {
			"pid_state_" + std::to_string(_pid_index) + "_" + i.first + "_index"
		};
		int index = ini_parser::instance().get<int>("pid_state", index_key);
		std::string prefix {
			i.first + "_pid_" + std::to_string(index)
		};
		i.second->update_coeff(
			ini_parser::instance().get<float>("pid_coeff", prefix + "_kp"),
			ini_parser::instance().get<float>("pid_coeff", prefix + "_ki"),
			ini_parser::instance().get<float>("pid_coeff", prefix + "_kd")
		);
	}
}

template <class T>
pid<T>& pid_manager<T>::get_pid(std::string name) {
	if (_pid_dataset.find(name) == _pid_dataset.end()) {
		throw std::out_of_range(
			"in pid_manager::get_pid(std::string name) : \"" + name + "\" is not found in \"_pid_dataset\""
		);
	}
	return *_pid_dataset.at(name);
}
