/*
 * arm_controller.cpp
 *
 *  Created on: 2016/10/05
 *      Author: u
 */

#include <controller/arm_controller.hpp>
#include <config.hpp>

std::string arm_controller::_state_name = "very_low";

arm_controller::arm_controller() {}

arm_controller::~arm_controller() {}

void arm_controller::update_arm() {
	update_grab();
	update_lock();

	adjustment();
	update_state_name();
	update_state_by_state_name();
}

void arm_controller::update_state_name() {
	int index = _state_index_dataset[_state_name];
	int arm_state_num = config::instance().get<int>("arm_state", "arm_state_num");
	float threshold = _command_threshold;

	if (is_key_rise("arm_state_index_increment", threshold) &&
			(index + 1 < arm_state_num)) {
		++index;
		for (auto&& i : _adjustment_dataset) {
			i.second = float();
		}
	}

	if (get_key_by_name("arm_state_index_increment") == get_key_by_name("arm_state_index_decrement")) {
		threshold *= -1.0f;
	}
	if (is_key_rise("arm_state_index_decrement", threshold) &&
			(index - 1 >= 0)) {
		--index;
		for (auto&& i : _adjustment_dataset) {
			i.second = float();
		}
	}

	std::string value_key {
		"state_" + std::to_string(index) + "_name"
	};
	_state_name = config::instance().get<std::string>("arm_state", value_key);
}

void arm_controller::adjustment() {
	for (const auto& i : _arm_ability_name_dataset) {
		if (is_key_rise(i + "_increse")) {
			std::string value_key {
				"state_" + std::to_string(_state_index_dataset[_state_name]) + "_" + i + "_index"
			};
			int index = config::instance().get<int>("arm_state", value_key);
			float pos = config::instance().get<float>(i, "position_" + std::to_string(index));
			_adjustment_dataset[i] += config::instance().get<float>("adjustment", i + "_coeff");
			if (pos + _adjustment_dataset[i] > 1.0f) {
				_adjustment_dataset[i] = 1.0f - pos;
			}

		}
		if (is_key_rise(i + "_decrese")) {
			std::string value_key {
				"state_" + std::to_string(_state_index_dataset[_state_name]) + "_" + i + "_index"
			};
			int index = config::instance().get<int>("arm_state", value_key);
			float pos = config::instance().get<float>(i, "position_" + std::to_string(index));
			_adjustment_dataset[i] -= config::instance().get<float>("adjustment", i + "_coeff");
			if (pos + _adjustment_dataset[i] < -1.0f) {
				_adjustment_dataset[i] = -1.0f - pos;
			}
		}
	}
}

void arm_controller::update_grab() {
	if (is_key_rise("grab")) {
		_is_grab_enable = !_is_grab_enable;
	}
}

void arm_controller::update_lock() {
	if (is_key_rise("lock")) {
		_is_lock_enable = !_is_lock_enable;
	}
}

void arm_controller::update_state_by_state_name() {
	for (const auto& i : _arm_ability_name_dataset) {
		std::string value_key {
			"state_" + std::to_string(_state_index_dataset[_state_name]) + "_" + i + "_index"
		};
		int index = config::instance().get<int>("arm_state", value_key);
		_arm_ability_position_index_dataset[i] = index;

		float pos = config::instance().get<float>(i, "position_" + std::to_string(index));
		pos += _adjustment_dataset[i];
		_command[i] = pos;
	}
}
