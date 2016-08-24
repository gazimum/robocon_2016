/*
 * dc_motor.hpp
 *
 *  Created on: 2016/08/23
 *      Author: u
 */

#ifndef DC_MOTOR_HPP_
#define DC_MOTOR_HPP_

#include <string>
#include <singleton.hpp>

class dc_motor : public singleton<dc_motor> {
public:
	static void set(std::string name, float p);
private:
	friend class singleton<dc_motor>;
	dc_motor() {}
};

#endif /* DC_MOTOR_HPP_ */
