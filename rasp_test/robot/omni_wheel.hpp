/*
 * omniwheel.h
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#ifndef OMNI_WHEEL_HPP_
#define OMNI_WHEEL_HPP_

#include <robot/moving_object.hpp>

class omni_wheel : public moving_object {
public:
	omni_wheel();
	virtual ~omni_wheel();

protected:
	virtual void write() override;
};

#endif /* OMNI_WHEEL_HPP_ */
