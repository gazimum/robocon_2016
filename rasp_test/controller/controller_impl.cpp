/*
 * controller_impl.cpp
 *
 *  Created on: 2016/07/18
 *      Author: u
 */

#include <controller/controller_impl.hpp>
#include <ini_parser.hpp>

const float controller_impl::_command_threshold = 0.5f;

controller_impl::controller_impl() {

}

controller_impl::~controller_impl() {

}

controller_impl* controller_impl::update(std::map<std::string, int>& controller_state) {
	std::map<std::string, float> normalized_controller_state;

	// 正規化 : コントローラの操作値の範囲を [-1.0f, 1.0f] にする
	for (const auto& i : controller_state) {
		std::string key {
			"normalization_coeff_" + i.first
		};
		normalized_controller_state[i.first] = i.second * ini_parser::instance().setting<float>(key);
	}

	controller_impl* state = update(normalized_controller_state);

	// ロボットの操作値に係数をかける
	for (const auto& i : _command) {
		std::string key {
			"command_coeff_" + i.first
		};
		_command[i.first] = i.second * ini_parser::instance().setting<float>(key);
	}

	return state;
}

float controller_impl::get(std::string key) {
	return _command[key];
}

