/*
 * controller_impl.cpp
 *
 *  Created on: 2016/07/18
 *      Author: u
 */

#include <config.hpp>
#include <controller/controller_impl.hpp>
#include <potentiometer.hpp>
#include <pid/pid_manager.hpp>

const float controller_impl::_command_threshold = 0.5f;
std::map<std::string, size_t> controller_impl::_arm_ability_position_index_dataset = {};
const std::vector<std::string> controller_impl::_arm_ability_name_dataset {
	"arm_length",
	"arm_height",
	"arm_angle"
};
bool controller_impl::_is_lock_enable = false;
bool controller_impl::_is_grab_enable = false;

controller_impl::controller_impl() {}

controller_impl::~controller_impl() {}

controller_impl* controller_impl::update(std::map<std::string, int>& controller_state) {
	// 正規化 : コントローラの操作値の範囲を [-1.0f, 1.0f] にする
	for (const auto& i : controller_state) {
		if (i.first.size() != 0) {
			std::string key {
				"normalization_coeff_" + i.first
			};
			_normalized_controller_state[i.first] = i.second * config::instance().get<float>("normalization_coeff", key);
		}
	}

	controller_impl* state = update();
	update_angle();
	update_config();
	update_pid_index();
	update_lpf_index();
	apply_grab();
	apply_lock();

	// ロボットの操作値に係数をかける
	for (auto&& i : _command) {
		i.second *= config::instance().get<float>("command_coeff", "command_coeff_" + i.first);
		i.second += config::instance().get<float>("command_offset", "command_offset_" + i.first);
	}

	_prev_normalized_controller_state = _normalized_controller_state;

	return state;
}

std::map<std::string, float> controller_impl::get_command() {
	return _command;
}

bool controller_impl::is_key_pushed(std::string name) {
	std::string key {
		get_key_by_name(name)
	};
	return (_normalized_controller_state[key] > _command_threshold);
}

bool controller_impl::is_key_rise(std::string name) {
	std::string key {
		get_key_by_name(name)
	};
	if (_normalized_controller_state[key] <= _command_threshold) {
		return false;
	}
	if (_prev_normalized_controller_state[key] > _command_threshold) {
		return false;
	}
	return true;
}

std::string controller_impl::get_key_by_name(std::string name) {
	return config::instance().get<std::string>("key_config", name);
}

int controller_impl::read_arm_ability_position_index() {
	size_t n = config::instance().get<int>("key_config", "arm_index_num");
	for (size_t i = 0; i < n; ++i) {
		if (is_key_pushed("arm_abilities_position_index_" + std::to_string(i))) {
			return i;
		}
	}
	return -1;
}

void controller_impl::update_config() {
	if (is_key_rise("reload_ini_file")) {
		config::instance().read();
		_command["reload_ini_file"] = 1.0f;
	} else {
		_command["reload_ini_file"] = -1.0f;
	}
}

void controller_impl::update_angle() {
	_command["angle_base_left"]  = potentiometer::instance().get_position("arm_height");
	_command["angle_base_right"] = potentiometer::instance().get_position("arm_height");
	_command["angle_left"]  = _command["arm_angle"];
	_command["angle_right"] = _command["arm_angle"];
}

void controller_impl::apply_grab() {
	if (_is_grab_enable) {
		_command["arm_width"] = 1.0f;
	} else {
		_command["arm_width"] = -1.0f;
	}
}

void controller_impl::apply_lock() {
	if (_is_lock_enable) {
		_command["lock"] = 1.0f;
	} else {
		_command["lock"] = -1.0f;
	}
}

