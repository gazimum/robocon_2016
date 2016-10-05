/*
 * fusion_fan_controller.hpp
 *
 *  Created on: 2016/10/04
 *      Author: u
 */

#ifndef CONTROLLER_FUSION_FAN_CONTROLLER_HPP_
#define CONTROLLER_FUSION_FAN_CONTROLLER_HPP_

#include <controller/controller_impl.hpp>

class fusion_fan_controller : public controller_impl {
public:
	virtual controller_impl* update() override;
	fusion_fan_controller();
	virtual ~fusion_fan_controller();
};

#endif /* CONTROLLER_FUSION_FAN_CONTROLLER_HPP_ */
