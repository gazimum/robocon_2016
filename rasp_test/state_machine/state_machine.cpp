/*
 * state_machine.cpp
 *
 *  Created on: 2016/08/11
 *      Author: u
 */

#include <state_machine/state_machine.hpp>

state_machine::state_machine(std::string initial_state) : _current_state_name(initial_state) {}

state_machine::~state_machine() {}

void state_machine::add_state(std::string name, std::function<std::string()> action) {
	_state_map[name] = action;
}

void state_machine::update() {
	_current_state_name = _state_map.at(_current_state_name)();
}

std::string state_machine::get_current_state_name() const {
	return _current_state_name;
}
