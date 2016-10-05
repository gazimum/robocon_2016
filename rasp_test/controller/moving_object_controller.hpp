/*
 * moving_object_controller.hpp
 *
 *  Created on: 2016/09/06
 *      Author: u
 */

#ifndef CONTROLLER_MOVING_OBJECT_CONTROLLER_HPP_
#define CONTROLLER_MOVING_OBJECT_CONTROLLER_HPP_

#include <controller/controller_impl.hpp>

class moving_object_controller : public virtual controller_impl {
public:
	moving_object_controller();
	virtual ~moving_object_controller();

protected:
	void update_movement();
};

#endif /* CONTROLLER_MOVING_OBJECT_CONTROLLER_HPP_ */
