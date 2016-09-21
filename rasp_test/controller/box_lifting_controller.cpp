/*
 * box_lifting_controller.cpp
 *
 *  Created on: 2016/09/21
 *      Author: u
 */

#include <controller/box_lifting_controller.hpp>

box_lifting_controller::box_lifting_controller() : _state_machine("wait_lifting") {
	_state_machine.add_state("wait_lifting", 	std::bind(&box_lifting_controller::wait_lifting, 	this));
	_state_machine.add_state("grab",			std::bind(&box_lifting_controller::grab, 	 		this));
	_state_machine.add_state("wait_release", 	std::bind(&box_lifting_controller::wait_release, 	this));
	_state_machine.add_state("lifting", 		std::bind(&box_lifting_controller::lifting, 		this));
}

box_lifting_controller::~box_lifting_controller() {
	// TODO Auto-generated destructor stub
}

controller_impl* box_lifting_controller::update() {
	_state_machine.update();
	return this;
}

std::string box_lifting_controller::wait_lifting() {
	if (true) {
		return "grab";
	}
	return "wait_lifting";
}

std::string box_lifting_controller::grab() {
	if (true) {
		return "wait_release";
	}
	return "grab";
}

std::string box_lifting_controller::wait_release() {
	if (true) {
		return "lifting";
	}
	return "wait_release";
}

std::string box_lifting_controller::lifting() {
	if (true) {
		return "wait_lifting";
	}
	return "lifting";
}
