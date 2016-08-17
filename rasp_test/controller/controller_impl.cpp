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
	// 正規化 : コントローラの操作値の範囲を [-1.0f, 1.0f] にする
	for (const auto& i : controller_state) {
		if (i.first.size() != 0) {
			std::string key {
				"normalization_coeff_" + i.first
			};
			_normalized_controller_state[i.first] = i.second * ini_parser::instance().get<float>("normalization_coeff", key);
		}
	}

	controller_impl* state = update();

	// ロボットの操作値に係数をかける
	for (const auto& i : _command) {
		_command[i.first] = i.second * ini_parser::instance().get<float>("command_coeff", "command_coeff_" + i.first);
		_command[i.first] += ini_parser::instance().get<float>("command_offset", "command_offset_" + i.first);
	}

	_prev_normalized_controller_state = _normalized_controller_state;

	return state;
}

float controller_impl::get(std::string key) {
	return _command[key];
}

