/*
 * arm.hpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#ifndef ARM_HPP_
#define ARM_HPP_

#include <pid/position_pid.hpp>
#include <map>
#include <string>

class arm {
public:
	typedef std::map<std::string, position_pid<float>> pid_container_type;

	arm();

	void update();
	void update_pid_coeff();

private:
	void update_angle();

	pid_container_type _pid;
};

#endif /* ARM_HPP_ */
