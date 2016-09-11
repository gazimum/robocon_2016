/*
 * config_pid.cpp
 *
 *  Created on: 2016/09/10
 *      Author: u
 */

#include <ini_parser.hpp>
#include <pid/pid_manager.hpp>

template <class T>
void pid_manager<T>::config() {
	for (auto&& i : _dataset) {
		std::string index_key {
			"pid_state_" + std::to_string(_index) + "_" + i.first + "_index"
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
