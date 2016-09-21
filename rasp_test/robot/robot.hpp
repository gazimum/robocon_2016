/*
 * robot.hpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#ifndef ROBOT_HPP_
#define ROBOT_HPP_

#include <memory>
#include <robot/arm.hpp>
#include <singleton.hpp>
#include <robot/moving_object.hpp>

class robot : public singleton<robot> {
public:
	void update();
	void update_moving_object();
	bool is_end();

private:
	friend class singleton<robot>;
	robot();

	std::unique_ptr<moving_object> _moving_object;
	arm _arm;
};

#endif /* ROBOT_HPP_ */
