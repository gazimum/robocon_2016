/*
 * simplecontroller.hpp
 *
 *  Created on: 2016/08/17
 *      Author: u
 */

#ifndef CONTROLLER_SIMPLE_CONTROLLER_HPP_
#define CONTROLLER_SIMPLE_CONTROLLER_HPP_

#include <controller/controller_impl.hpp>
#include <state_machine/state_machine.hpp>
#include <chrono>

class simple_controller : public controller_impl {
public:
	simple_controller();
	virtual ~simple_controller();

private:
	virtual controller_impl* update() override;
	controller_impl* update_sequence();

	std::string release();
	std::string height_low();
	std::string grab();
	std::string height_adjust();

	void update_height_by_index_and_adjust();

	void update_height_index();
	void update_height_adjust();

	static const double _release_wait_time_ms;

	std::chrono::system_clock::time_point _time;

	size_t _height_index;
	float _height_adjust;

	state_machine _state_machine;
};

#endif /* CONTROLLER_SIMPLE_CONTROLLER_HPP_ */
