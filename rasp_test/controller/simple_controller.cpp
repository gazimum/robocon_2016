/*
 * simplecontroller.cpp
 *
 *  Created on: 2016/08/17
 *      Author: u
 */

#include <controller/simple_controller.hpp>
#include <controller/basic_controller.hpp>
#include <ini_parser.hpp>
#include <string>

const float simple_controller::_release_wait_time_ms = 6500.0f;

simple_controller::simple_controller() : _state_machine("release") {
	_state_machine.add_state("release", 		std::bind(&simple_controller::release, 		 this));
	_state_machine.add_state("height_low",		std::bind(&simple_controller::height_low, 	 this));
	_state_machine.add_state("grab", 			std::bind(&simple_controller::grab, 		 this));
	_state_machine.add_state("height_adjust", 	std::bind(&simple_controller::height_adjust, this));

	_time = std::chrono::system_clock::now();
}

simple_controller::~simple_controller() {
	// TODO Auto-generated destructor stub
}

controller_impl* simple_controller::update() {
	_state_machine.update();
	return update_sequence();
}

controller_impl* simple_controller::update_sequence() {
	std::string key[] {
		ini_parser::instance().get<std::string>("key_config", "controller_switch_1"),
		ini_parser::instance().get<std::string>("key_config", "controller_switch_2")
	};

	bool is_change_key_pressed = (_normalized_controller_state[key[0]] > _command_threshold);
	is_change_key_pressed |= (_normalized_controller_state[key[1]] > _command_threshold);

	bool was_change_key_pressed = (_prev_normalized_controller_state[key[0]] > _command_threshold);
	is_change_key_pressed |= (_prev_normalized_controller_state[key[1]] > _command_threshold);

	if (is_change_key_pressed && !was_change_key_pressed) {
		return new basic_controller();
	}

	return this;
}

std::string simple_controller::release() {
	_command["width"] = -1.0f;

	double elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(_time - std::chrono::system_clock::now()).count();
	if (elapsed_ms > _release_wait_time_ms) {
		return "height_low";
	}

	return "release";
}

std::string simple_controller::height_low() {
	_height_index = 0;
	_height_adjust = 0.0f;
	update_height_by_index_and_adjust();

	std::string grab_key {
		ini_parser::instance().get<std::string>("key_config", "grab")
	};
	if ((     _normalized_controller_state[grab_key] >  _command_threshold) &&
		(_prev_normalized_controller_state[grab_key] <= _command_threshold)){
		_time = std::chrono::system_clock::now();
		return "grab";
	}

	return "height_low";
}

std::string simple_controller::grab() {
	_command["width"] = 1.0f;

	double elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(_time - std::chrono::system_clock::now()).count();
	if (elapsed_ms > _release_wait_time_ms) {
		_height_index = 1;
		return "height_adjust";
	}

	return "grab";
}

std::string simple_controller::height_adjust() {
	update_height_index();
	update_height_adjust();
	update_height_by_index_and_adjust();

	std::string grab_key {
		ini_parser::instance().get<std::string>("key_config", "grab")
	};
	if ((     _normalized_controller_state[grab_key] >  _command_threshold) &&
		(_prev_normalized_controller_state[grab_key] <= _command_threshold)){
		_time = std::chrono::system_clock::now();
		return "release";
	}

	return "height_adjust";
}

void simple_controller::update_height_by_index_and_adjust() {
	_command["height"] = ini_parser::instance().get<float>("arm_height", "position" + std::to_string(_height_index));
	_command["height"] += _height_adjust;
}

void simple_controller::update_height_index() {


	std::string grab_key {
		ini_parser::instance().get<std::string>("key_config", "grab")
	};
	if ((     _normalized_controller_state[grab_key] >  _command_threshold) &&
		(_prev_normalized_controller_state[grab_key] <= _command_threshold)){
		_time = std::chrono::system_clock::now();
		return "grab";
	}
}

void simple_controller::update_height_adjust() {

}
