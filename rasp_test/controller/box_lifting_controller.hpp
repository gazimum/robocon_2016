/*
 * box_lifting_controller.hpp
 *
 *  Created on: 2016/09/21
 *      Author: u
 */

#ifndef CONTROLLER_BOX_LIFTING_CONTROLLER_HPP_
#define CONTROLLER_BOX_LIFTING_CONTROLLER_HPP_

#include <controller/controller_impl.hpp>
#include <state_machine/state_machine.hpp>

class box_lifting_controller : public controller_impl {
public:
	virtual controller_impl* update() override;
	box_lifting_controller();
	virtual ~box_lifting_controller();

private:
	std::string wait_lifting();
	std::string grab();
	std::string wait_release();
	std::string lifting();

	state_machine _state_machine;
};

#endif /* CONTROLLER_BOX_LIFTING_CONTROLLER_HPP_ */
