/*
 * controller_impl.cpp
 *
 *  Created on: 2016/07/18
 *      Author: u
 */

#include <controller/controller_impl.hpp>
#include <ini_parser.hpp>

const float controller_impl::_command_threshold = 0.5f;

controller_impl::controller_impl() {}

controller_impl::~controller_impl() {}

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
	for (auto&& i : _command) {
		i.second *= ini_parser::instance().get<float>("command_coeff", "command_coeff_" + i.first);
		i.second += ini_parser::instance().get<float>("command_offset", "command_offset_" + i.first);
	}

	_prev_normalized_controller_state = _normalized_controller_state;

	return state;
}

float controller_impl::get(std::string key) {
	return _command[key];
}

bool controller_impl::is_key_pushed(std::string key) {
	return (_normalized_controller_state[key] > _command_threshold);
}

bool controller_impl::is_key_rise(std::string key) {
	if (_normalized_controller_state[key] <= _command_threshold) {
		return false;
	}
	if (_prev_normalized_controller_state[key] > _command_threshold) {
		return false;
	}
	return true;
}


int controller_impl::read_arm_abilities_position_index() {
	int n = ini_parser::instance().get<int>("key_config", "arm_index_num");
	for (size_t i = 0; i < n; ++i) {
		std::string key {
			ini_parser::instance().get<std::string>("key_config", "arm_abilities_position_index_" + std::to_string(i))
		};
		if (is_key_pushed(key)) {
			return i;
		}
	}
	return -1;
}

